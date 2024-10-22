#include <PanicEngine/Inc/PanicEngine.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Math;

struct Arguments
{
    std::filesystem::path inputFileName;
    std::filesystem::path outputFileName;
    float scale = 1.0f;
};

Vector3 ToVector3(const aiVector3D& v)
{
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        static_cast<float>(v.z)
    };
}

Vector2 ToTexCoord(const aiVector3D& v)
{
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y)
    };
}

std::optional<Arguments> ParseArgs(int argc, char* argv[])
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
    }

    return args;
}

int main(int argc, char* argv[])
{
    const auto argOpt = ParseArgs(argc, argv);
    if (!argOpt.has_value())
    {
        printf("Not enough arguments, import fbx failed!\n");
    }

    printf("Begin import.\n");
    const Arguments& args = argOpt.value();

    // use assimp
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;

    const aiScene* scene = importer.ReadFile(args.inputFileName.u8string().c_str(), flags);
    if (scene == nullptr)
    {
        printf("Error: %s\n", importer.GetErrorString());
    }

    printf("Importing %s...\n", args.inputFileName.u8string().c_str());

    Model model;

    if (scene->HasMeshes())
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
        }
    }

    printf("Saving Model...\n");
    ModelIO::SaveModel(args.outputFileName, model);

    return 0;
}