#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"

namespace PanicEngine::Graphics
{
	class Camera;
	class RenderObject;
	class RenderGroup;
	class Texture;

	class StandardEffect final
	{
	public:
		void Initialize(const std::filesystem::path& path);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void Render(const RenderGroup& renderGroup);

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture& shadowMap);

		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Matrix4 lwvp;	// light world view projection matrix
			Math::Matrix4 world;
			Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;	// using int to keep it 16 byte aligned
			int useNormalMap = 1;
			int useSpecMap = 1;
			int useBumpMap = 1;
			int useShadowMap = 1;
			int useSkinning = 1;
			float bumpWeight = 0.0f;
			float depthBias = 0.000003f;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		using BoneTransformBuffer = ConstantBuffer;

		TransformBuffer mTransformBuffer;
		LightBuffer mLightBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingsBuffer mSettingsBuffer;
		BoneTransformBuffer mBoneTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}