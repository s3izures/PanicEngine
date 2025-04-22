#include "Precompiled.h"
#include "RenderService.h"

#include "CameraService.h"
#include "RenderObjectComponent.h"
#include "TransformComponent.h"

#include "GameWorld.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;

void RenderService::Initialize()
{
    mCameraService = GetWorld().GetService<CameraService>();

    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);
}

void RenderService::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
    mFPS = 1.0f / deltaTime;
}

void RenderService::Render()
{
    const Camera& camera = mCameraService->GetMain();
    mStandardEffect.SetCamera(camera);
    for (Entry& entry : mRenderEntries)
    {
        entry.renderGroup.transform = *entry.transformComponent;
    }

    mShadowEffect.Begin();
        for (Entry& entry : mRenderEntries)
        {
            if (entry.renderComponent->CanCastShadow())
            {
                mShadowEffect.Render(entry.renderGroup);
            }
        }
    mShadowEffect.End();

    mStandardEffect.Begin();
        for (Entry& entry : mRenderEntries)
        {
            mStandardEffect.Render(entry.renderGroup);
        }
    mStandardEffect.End();
}

void RenderService::DebugUI()
{
    if (ImGui::CollapsingHeader("RenderService"))
    {
        ImGui::Text("FPS: %.3f", mFPS);

        if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Direction", &mDirectionalLight.direction.x, 0.01f))
            {
                mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
            }

            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }

        mStandardEffect.DebugUI();
        mShadowEffect.DebugUI();
    }
}

void RenderService::Register(const RenderObjectComponent* renderObjectComponent)
{
    auto iter = std::find_if(
                            mRenderEntries.begin(), 
                            mRenderEntries.end(), 
                            [&](const Entry& entry) {return entry.renderComponent == renderObjectComponent;});

    if (iter == mRenderEntries.end())
    {
        Entry& entry = mRenderEntries.emplace_back();
        entry.renderComponent = renderObjectComponent;
        entry.transformComponent = renderObjectComponent->GetOwner().GetComponent<TransformComponent>();
        entry.renderGroup.Initialize(renderObjectComponent->GetModel());
        entry.renderGroup.modelId = renderObjectComponent->GetModelId();
    }
}

void RenderService::Unregister(const RenderObjectComponent* renderObjectComponent)
{
    auto iter = std::find_if(
        mRenderEntries.begin(),
        mRenderEntries.end(),
        [&](const Entry& entry) {return entry.renderComponent == renderObjectComponent;});

    if (iter != mRenderEntries.end())
    {
        iter->renderGroup.Terminate();
        mRenderEntries.erase(iter);
    }
}
