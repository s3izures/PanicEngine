#include "Precompiled.h"
#include "StandardEffect.h"
#include "VertexTypes.h"
#include "Camera.h"
#include "RenderObject.h"
#include "AnimationUtil.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

static constexpr size_t MaxBoneCount = 256;

void StandardEffect::Initialize(const std::filesystem::path& path)
{
	mVertexShader.Initialize<Vertex>(path);
	mPixelShader.Initialize(path);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mBoneTransformBuffer.Initialize(MaxBoneCount * sizeof(Math::Matrix4));
}

void StandardEffect::Terminate()
{
	mBoneTransformBuffer.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	// (0) -> register 0
	// (1) -> register 1
	mLightBuffer.BindVS(1);	// direction
	mLightBuffer.BindPS(1);	// colors

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindPS(3);
	mSettingsBuffer.BindVS(3);

	mBoneTransformBuffer.BindVS(4);
}

void StandardEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(4);
	}
}

void StandardEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "StandardEffect: must have a camera");
	SettingsData settingsData;
	settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0;
	settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
	settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0;
	settingsData.useBumpMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0;
	settingsData.bumpWeight = mSettingsData.bumpWeight;
	settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
	settingsData.depthBias = mSettingsData.depthBias;
	settingsData.useSkinning = 0;

	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData transformData;
	transformData.wvp = Transpose(matFinal);
	transformData.world = Transpose(matWorld);
	transformData.viewPosition = mCamera->GetPosition();
	if (settingsData.useShadowMap)
	{
		const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
		const Math::Matrix4 matLightProjection = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = Transpose(matWorld * matLightView * matLightProjection);
		mShadowMap->BindPS(4);
	}

	mSettingsBuffer.Update(settingsData);
	mTransformBuffer.Update(transformData);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);

	TextureCache* tc = TextureCache::Get();
	tc->BindPS(renderObject.diffuseMapId, 0);
	tc->BindPS(renderObject.normalMapId, 1);
	tc->BindPS(renderObject.specMapId, 2);
	tc->BindVS(renderObject.bumpMapId, 3);

	renderObject.meshBuffer.Render();
}

void StandardEffect::Render(const RenderGroup& renderGroup)
{
	ASSERT(mCamera != nullptr, "StandardEffect: must have a camera");

	const Math::Matrix4 matWorld = renderGroup.transform.GetMatrix4();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	SettingsData settingsData;
	settingsData.useShadowMap = mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
	settingsData.depthBias = mSettingsData.depthBias;
	settingsData.bumpWeight = mSettingsData.bumpWeight;
	settingsData.useSkinning = mSettingsData.useSkinning > 0 && renderGroup.skeleton != nullptr;

	TransformData transformData;
	transformData.wvp = Transpose(matFinal);
	transformData.world = Transpose(matWorld);
	transformData.viewPosition = mCamera->GetPosition();
	if (settingsData.useShadowMap)
	{
		const Math::Matrix4 matLightView = mLightCamera->GetViewMatrix();
		const Math::Matrix4 matLightProjection = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = Transpose(matWorld * matLightView * matLightProjection);
		mShadowMap->BindPS(4);
	}

	if (settingsData.useSkinning)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransforms(renderGroup.modelId, boneTransforms, renderGroup.animator);
		AnimationUtil::ApplyBoneOffset(renderGroup.modelId, boneTransforms);

		// our engine is left handed
		// directX is right handed
		for (auto& transform : boneTransforms)
		{
			transform = Transpose(transform);
		}
		boneTransforms.resize(MaxBoneCount);
		mBoneTransformBuffer.Update(boneTransforms.data());
	}

	mTransformBuffer.Update(transformData);

	mLightBuffer.Update(*mDirectionalLight);

	for (const RenderObject& renderObject : renderGroup.renderObjects)
	{
		mMaterialBuffer.Update(renderObject.material);

		settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0;
		settingsData.useNormalMap = mSettingsData.useNormalMap > 0 && renderObject.normalMapId > 0;
		settingsData.useSpecMap = mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0;
		settingsData.useBumpMap = mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0;

		mSettingsBuffer.Update(settingsData);

		TextureCache* tc = TextureCache::Get();
		tc->BindPS(renderObject.diffuseMapId, 0);
		tc->BindPS(renderObject.normalMapId, 1);
		tc->BindPS(renderObject.specMapId, 2);
		tc->BindVS(renderObject.bumpMapId, 3);

		renderObject.meshBuffer.Render();
	}
}

void StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StandardEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void StandardEffect::SetShadowMap(const Texture& shadowMap)
{
	mShadowMap = &shadowMap;
}

void StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuse = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("UseDiffuse", &useDiffuse))
		{
			mSettingsData.useDiffuseMap = (useDiffuse) ? 1 : 0;
		}

		bool useNormal = mSettingsData.useNormalMap > 0;
		if (ImGui::Checkbox("UseNormal", &useNormal))
		{
			mSettingsData.useNormalMap = (useNormal) ? 1 : 0;
		}

		bool useSpec = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("UseSpec", &useSpec))
		{
			mSettingsData.useSpecMap = (useSpec) ? 1 : 0;
		}

		bool useBump = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("UseBump", &useBump))
		{
			mSettingsData.useBumpMap = (useBump) ? 1 : 0;
		}
		ImGui::DragFloat("BumpWeight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 10000.0f);

		bool useShadowMap = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("UseShadowMap", &useShadowMap))
		{
			mSettingsData.useShadowMap = useShadowMap ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");

		bool useSkinning = mSettingsData.useSkinning > 0;
		if (ImGui::Checkbox("UseSkinningMap", &useSkinning))
		{
			mSettingsData.useSkinning = useSkinning ? 1 : 0;
		}
	}
}
