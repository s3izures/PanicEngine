#include "Precompiled.h"
#include "RenderService.h"

#include "CameraService.h"
#include "AnimatorComponent.h"
#include "RenderObjectComponent.h"
#include "TransformComponent.h"

#include "GameWorld.h"

using namespace PanicEngine;
using namespace PanicEngine::Math;
using namespace PanicEngine::Graphics;

void RenderService::Initialize()
{
    mCameraService = GetWorld().GetService<CameraService>();

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
        const Graphics::Animator* animator = nullptr;

        const AnimatorComponent* animatorComponent = renderObjectComponent->GetOwner().GetComponent<AnimatorComponent>();

        if (animatorComponent != nullptr)
        {
            animator = &animatorComponent->GetAnimator();
        }

        Entry& entry = mRenderEntries.emplace_back();
        entry.renderComponent = renderObjectComponent;
        entry.transformComponent = renderObjectComponent->GetOwner().GetComponent<TransformComponent>();
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

void PanicEngine::RenderService::Deserialize(const rapidjson::Value& value)
{
    mDirectionalLight.direction = { 1.0f, -1.0f, 1.0f };
    mDirectionalLight.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

    if (value.HasMember("Direction"))
    {
        auto dir = value["Direction"].GetArray();
        mDirectionalLight.direction.x = dir[0].GetFloat();
        mDirectionalLight.direction.y = dir[1].GetFloat();
        mDirectionalLight.direction.z = dir[2].GetFloat();
    }
    if (value.HasMember("Ambient"))
    {
        auto color = value["Ambient"].GetArray();
        mDirectionalLight.ambient.r = color[0].GetFloat();
        mDirectionalLight.ambient.g = color[1].GetFloat();
        mDirectionalLight.ambient.b = color[2].GetFloat();
        mDirectionalLight.ambient.a = color[3].GetFloat();
    }
    if (value.HasMember("Diffuse"))
    {
        auto color = value["Diffuse"].GetArray();
        mDirectionalLight.diffuse.r = color[0].GetFloat();
        mDirectionalLight.diffuse.g = color[1].GetFloat();
        mDirectionalLight.diffuse.b = color[2].GetFloat();
        mDirectionalLight.diffuse.a = color[3].GetFloat();
    }
    if (value.HasMember("Specular"))
    {
        auto color = value["Specular"].GetArray();
        mDirectionalLight.specular.r = color[0].GetFloat();
        mDirectionalLight.specular.g = color[1].GetFloat();
        mDirectionalLight.specular.b = color[2].GetFloat();
        mDirectionalLight.specular.a = color[3].GetFloat();
    }
    mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
}
