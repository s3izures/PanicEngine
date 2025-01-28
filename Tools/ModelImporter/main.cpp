#include <PanicEngine/Inc/PanicEngine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Math;

using BoneIndexLookup = std::map<std::string, uint32_t>;

struct Arguments
{
	std::filesystem::path inputFileName;
	std::filesystem::path outputFileName;
	float scale = 1.0f;
	bool animOnly = false;
};

Vector3 ToVector3(const aiVector3D& v)
{
	return {
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

Quaternion ToQuaternion(const aiQuaternion& q)
{
	return {
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z),
		static_cast<float>(q.w)
	};
}

Vector2 ToTexCoord(const aiVector3D& v)
{
	return {
		static_cast<float>(v.x),
		static_cast<float>(v.y)
	};
}

Color ToColor(const aiColor3D& c)
{
	return {
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

Matrix4 ToMatrix4(const aiMatrix4x4& m)
{
	return {
		static_cast<float>(m.a1), static_cast<float>(m.b1), static_cast<float>(m.c1), static_cast<float>(m.d1),
		static_cast<float>(m.a2), static_cast<float>(m.b2), static_cast<float>(m.c2), static_cast<float>(m.d2),
		static_cast<float>(m.a3), static_cast<float>(m.b3), static_cast<float>(m.c3), static_cast<float>(m.d3),
		static_cast<float>(m.a4), static_cast<float>(m.b4), static_cast<float>(m.c4), static_cast<float>(m.d4)
	};
}

std::optional<Arguments> ParserArgs(int argc, char* argv[])
{
	if (argc < 3)
	{
		return std::nullopt;
	}

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	for (int i = 1; i + 2 < argc; ++i)
	{
		if (strcmp(argv[i], "-scale") == 0)
		{
			args.scale = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "-animOnly") == 0)
		{
			args.animOnly = argv[i + 1] == "1";
			++i;
		}
	}

	return args;
}

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args,
	const std::filesystem::path& fileName)
{
	printf("Extracting embedded texture %s\n", fileName.u8string().c_str());

	std::string fullFileName = args.outputFileName.u8string();
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName.filename().u8string();

	FILE* file = nullptr;
	auto err = fopen_s(&file, fullFileName.c_str(), "wb");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving\n", fullFileName.c_str());
		return;
	}

	size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
	ASSERT(written == texture->mWidth, "Error: failed to extract embedded texture");
	fclose(file);
}

std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial,
	aiTextureType textureType, const Arguments& args, const std::string& suffix,
	uint32_t materialIndex)
{
	const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
	if (textureCount == 0)
	{
		return "";
	}

	std::filesystem::path textureName;
	aiString texturePath;
	if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
	{
		if (texturePath.C_Str()[0] == '*')	// if first character == *  , it is an embedded texture
		{
			std::string fileName = args.inputFileName.u8string();
			fileName.erase(fileName.length() - 4);	// erasing .fbx
			fileName += suffix;
			fileName += texturePath.C_Str()[1];

			ASSERT(scene->HasTextures(), "Error: no embedded texture found");

			int textureIndex = atoi(texturePath.C_Str() + 1);
			ASSERT(textureIndex < scene->mNumTextures, "Error: invalid texture index");

			const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
			ASSERT(embeddedTexture->mHeight == 0, "Error: uncompressed texture found");

			if (embeddedTexture->CheckFormat("jpg"))
			{
				fileName += ".jpg";
			}
			else if (embeddedTexture->CheckFormat("png"))
			{
				fileName += ".png";
			}
			else
			{
				ASSERT(false, "Error: unrecognized texture format");
			}

			ExportEmbeddedTexture(embeddedTexture, args, fileName);
			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
		else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
		{
			std::filesystem::path embeddedFilePath = texturePath.C_Str();
			std::string fileName = args.inputFileName.u8string();
			fileName.erase(fileName.length() - 4);
			fileName += suffix;
			fileName += "_" + std::to_string(materialIndex);
			fileName += embeddedFilePath.extension().u8string();

			ExportEmbeddedTexture(embeddedTexture, args, fileName);
			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
		else
		{
			std::filesystem::path filePath = texturePath.C_Str();
			std::string fileName = filePath.filename().u8string();

			printf("Adding texture %s\n", fileName.c_str());
			textureName = fileName;
		}
	}

	return textureName.filename().u8string();
}

Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	Bone* bone = nullptr;
	std::string boneName = sceneNode.mName.C_Str();
	auto iter = boneIndexLookup.find(boneName);
	if (iter != boneIndexLookup.end())
	{
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		if (boneName.empty())
		{
			bone->name = "NoName" + std::to_string(bone->index);
		}
		else
		{
			bone->name = std::move(boneName);
		}
		boneIndexLookup.emplace(bone->name, bone->index);

		bone->parent = parent;
		bone->parentIndex = parent ? parent->index : -1;
		bone->toParentTransform = ToMatrix4(sceneNode.mTransformation);
		bone->offsetTransform = Matrix4::Identity;

		if (skeleton.root == nullptr && parent == nullptr)
		{
			skeleton.root = bone;
		}

		bone->children.reserve(sceneNode.mNumChildren);
		for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
		{
			Bone* child = BuildSkeleton(*(sceneNode.mChildren[i]), bone, skeleton, boneIndexLookup);
			bone->children.push_back(child);
			bone->childrenIndices.push_back(child->index);
		}
	}

	return bone;
}

uint32_t GetBoneIndex(const aiBone* nodeBone, const BoneIndexLookup& boneIndexMap)
{
	std::string boneName = nodeBone->mName.C_Str();
	auto iter = boneIndexMap.find(boneName);
	ASSERT(iter != boneIndexMap.end(), "ERROR: aiBone was not found in the index map");
	return iter->second;
}

void SetBoneOffsetTransform(const aiBone* nodeBone, Skeleton& skeleton, const BoneIndexLookup& boneIndexMap)
{
	uint32_t boneIndex = GetBoneIndex(nodeBone, boneIndexMap);
	Bone* bone = skeleton.bones[boneIndex].get();
	bone->offsetTransform = ToMatrix4(nodeBone->mOffsetMatrix);
}

int main(int argc, char* argv[])
{
	const auto argOpt = ParserArgs(argc, argv);
	if (!argOpt.has_value())
	{
		printf("Not enough arguments, import fbx failed\n");
		return -1;
	}

	printf("Begin Import\n");
	const Arguments& args = argOpt.value();

	// use assimp
	Assimp::Importer importer;
	// for importing skeletons properly
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;

	const aiScene* scene = importer.ReadFile(args.inputFileName.u8string().c_str(), flags);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	printf("Importing %s...\n", args.inputFileName.u8string().c_str());

	Model model;
	BoneIndexLookup boneIndexLookup;

	if (scene->HasMeshes())
	{
		printf("Reading skeleton...\n");
		model.skeleton = std::make_unique<Skeleton>();
		BuildSkeleton(*scene->mRootNode, nullptr, *model.skeleton, boneIndexLookup);
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
		{
			const auto& aiMesh = scene->mMeshes[meshIndex];
			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			{
				continue;
			}
			if (aiMesh->HasBones())
			{
				for (uint32_t b = 0; b < aiMesh->mNumBones; ++b)
				{
					const auto& aiBone = aiMesh->mBones[b];
					SetBoneOffsetTransform(aiBone, *model.skeleton, boneIndexLookup);
				}
			}
		}

		for (auto& bone : model.skeleton->bones)
		{
			bone->offsetTransform._41 *= args.scale;
			bone->offsetTransform._42 *= args.scale;
			bone->offsetTransform._43 *= args.scale;
			bone->toParentTransform._41 *= args.scale;
			bone->toParentTransform._42 *= args.scale;
			bone->toParentTransform._43 *= args.scale;
		}

		if(!args.animOnly)
		{
			printf("Reading Mesh Data...\n");
			for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
			{
				const auto& aiMesh = scene->mMeshes[meshIndex];
				if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
				{
					continue;
				}

				const uint32_t numVertices = aiMesh->mNumVertices;
				const uint32_t numFaces = aiMesh->mNumFaces;
				const uint32_t numIndices = numFaces * 3;

				Model::MeshData& meshData = model.meshData.emplace_back();

				printf("Reading Material Index...\n");
				meshData.materialIndex = aiMesh->mMaterialIndex;

				printf("Reading Vertices...\n");
				Mesh& mesh = meshData.mesh;
				mesh.vertices.reserve(numVertices);

				const aiVector3D* positions = aiMesh->mVertices;
				const aiVector3D* normals = aiMesh->mNormals;
				const aiVector3D* tangents = (aiMesh->HasTangentsAndBitangents()) ? aiMesh->mTangents : nullptr;
				const aiVector3D* texCoords = (aiMesh->HasTextureCoords(0)) ? aiMesh->mTextureCoords[0] : nullptr;

				for (uint32_t v = 0; v < numVertices; ++v)
				{
					Vertex& vertex = mesh.vertices.emplace_back();
					vertex.position = ToVector3(positions[v]) * args.scale;
					vertex.normal = ToVector3(normals[v]);
					vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
					vertex.uvCoord = texCoords ? ToTexCoord(texCoords[v]) : Vector2::Zero;
				}

				printf("Reading Indices...\n");
				mesh.indices.reserve(numIndices);
				const auto& aiFaces = aiMesh->mFaces;
				for (uint32_t f = 0; f < numFaces; ++f)
				{
					const auto& aiFace = aiFaces[f];
					for (uint32_t i = 0; i < 3; ++i)
					{
						mesh.indices.push_back(aiFace.mIndices[i]);
					}
				}

				if (aiMesh->HasBones())
				{
					printf("Reading bone weight...\n");
					std::vector<int> numWeightsAdded(mesh.vertices.size());
					for (uint32_t b = 0; b < aiMesh->mNumBones; ++b)
					{
						const auto& aiBone = aiMesh->mBones[b];
						uint32_t boneIndex = GetBoneIndex(aiBone, boneIndexLookup);
						for (uint32_t w = 0; w < aiBone->mNumWeights; ++w)
						{
							const aiVertexWeight& weight = aiBone->mWeights[w];
							Vertex& v = mesh.vertices[weight.mVertexId];
							int& count = numWeightsAdded[weight.mVertexId];
							if (count < Vertex::MaxBoneWeights)
							{
								v.boneIndices[count] = boneIndex;
								v.boneWeights[count] = weight.mWeight;
								++count;
							}
						}
					}
				}
			}
		}
	}

	if (args.animOnly)
	{
		if (scene->HasMeshes())
		{
			printf("Reading Material Data...\n");

			const uint32_t numMaterials = scene->mNumMaterials;
			model.materialData.reserve(numMaterials);
			for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
			{
				const auto aiMaterial = scene->mMaterials[materialIndex];
				aiColor3D ambient, diffuse, emissive, specular;
				ai_real specularPower = 1.0f;
				aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
				aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
				aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
				aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
				aiMaterial->Get(AI_MATKEY_SHININESS, specularPower);

				Model::MaterialData& materialData = model.materialData.emplace_back();
				materialData.material.ambient = ToColor(ambient);
				materialData.material.diffuse = ToColor(diffuse);
				materialData.material.emissive = ToColor(emissive);
				materialData.material.specular = ToColor(specular);
				materialData.material.power = static_cast<float>(specularPower);

				materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, args, "_diff", materialIndex);
				materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, args, "_norm", materialIndex);
				materialData.specMapName = FindTexture(scene, aiMaterial, aiTextureType_SPECULAR, args, "_spec", materialIndex);
				materialData.bumpMapName = FindTexture(scene, aiMaterial, aiTextureType_DISPLACEMENT, args, "_bump", materialIndex);
			}
		}
	}

	if (scene->HasAnimations())
	{
		printf("Building Animations...\n");
		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const auto& aiAnim = scene->mAnimations[animIndex];
			AnimationClip& animClip = model.animationClips.emplace_back();
			if (aiAnim->mName.length > 0)
			{
				animClip.name = aiAnim->mName.C_Str();
			}
			else
			{
				animClip.name = "Anim" + std::to_string(animIndex);
			}

			animClip.tickDuration = static_cast<float>(aiAnim->mDuration);
			animClip.ticksPerSecond = static_cast<float>(aiAnim->mTicksPerSecond);

			printf("Reading bone animation for %s...\n", animClip.name.c_str());
			animClip.boneAnimations.resize(model.skeleton->bones.size());
			for (uint32_t boneAnimIndex = 0;boneAnimIndex < aiAnim->mNumChannels;++boneAnimIndex)
			{
				const auto& aiBoneAnim = aiAnim->mChannels[boneAnimIndex];
				const int boneIndex = boneIndexLookup[aiBoneAnim->mNodeName.C_Str()];
				auto& boneAnimation = animClip.boneAnimations[boneIndex];
				boneAnimation = std::make_unique<Animation>();

				AnimationBuilder builder;

				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					const aiVectorKey& pos = aiBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(ToVector3(pos.mValue) * args.scale, static_cast<float>(pos.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					const aiQuatKey& rot = aiBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(ToQuaternion(rot.mValue), static_cast<float>(rot.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					const aiVectorKey& scale = aiBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(ToVector3(scale.mValue), static_cast<float>(scale.mTime));
				}

				*boneAnimation = builder.Build();
			}
		}
	}

	if(!args.animOnly)
	{
		printf("Saving Model...\n");
		ModelIO::SaveModel(args.outputFileName, model);

		printf("Saving Material...\n");
		ModelIO::SaveMaterial(args.outputFileName, model);

		printf("Saving Skeleton...\n");
		ModelIO::SaveSkeleton(args.outputFileName, model);
	}

	printf("Saving Animations...\n");
	ModelIO::SaveAnimations(args.outputFileName, model);

	return 0;
}