#include "Precompiled.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"

#include "CameraService.h"
#include "RenderService.h"
#include "PhysicsService.h"
#include "UIRenderService.h"

using namespace PanicEngine;

namespace
{
    CustomService TryAddService;
    std::filesystem::path sEditTemplateFile;
}

void GameWorld::SetCustomService(CustomService customService)
{
    TryAddService = customService;
}

void GameWorld::Initialize(uint32_t capacity)
{
    ASSERT(!mInitialized, "GameWorld: already initialized");
    for (auto& service : mServices)
    {
        service->Initialize();
    }

    mGameObjectSlots.resize(capacity);
    mFreeSlots.resize(capacity);
    std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);

    mInitialized = true;
}

void GameWorld::Terminate()
{
    for (Slot& slot : mGameObjectSlots)
    {
        if (slot.gameObject != nullptr)
        {
            slot.gameObject->Terminate();
            slot.gameObject.reset();
        }
    }
    mGameObjectSlots.clear();

    for (auto& service : mServices)
    {
        service->Terminate();
        service.reset();
    }
    mServices.clear();
}

void GameWorld::Update(float deltaTime)
{
    for (Slot& slot : mGameObjectSlots)
    {
        if (slot.gameObject != nullptr)
        {
            slot.gameObject->Update(deltaTime);
        }
    }

    for (auto& service : mServices)
    {
        service->Update(deltaTime);
    }

    ProcessDestroyList();
}

void GameWorld::Render()
{
    for (auto& service : mServices)
    {
        service->Render();
    }
}

void GameWorld::DebugUI()
{
    for (Slot& slot : mGameObjectSlots)
    {
        if (slot.gameObject != nullptr)
        {
            slot.gameObject->DebugUI();
        }
    }

    for (auto& service : mServices)
    {
        service->DebugUI();
    }

    if (mIsEditor)
    {
        if (ImGui::Button("Save"))
        {
            SaveEditTemplate();
        }
    }
}

GameObject* GameWorld::CreateGameObject(std::string name, const std::filesystem::path& templatePath)
{
    ASSERT(mInitialized, "GameWorld: is not initialized");
    if (mFreeSlots.empty())
    {
        ASSERT(false, "GameWorld: no free slots available");
        return nullptr;
    }
    const uint32_t freeSlot = mFreeSlots.back();
    mFreeSlots.pop_back();

    Slot& slot = mGameObjectSlots[freeSlot];
    auto& newGO = slot.gameObject;
    newGO = std::make_unique<GameObject>();
    newGO->SetName(name);
    newGO->mWorld = this;
    newGO->mHandle.mIndex = freeSlot;
    newGO->mHandle.mGeneration = slot.generation;
    if (!templatePath.empty())
    {
        GameObjectFactory::Make(templatePath, *newGO, *this);
    }
    return newGO.get();
}

void GameWorld::DestroyGameObject(const GameObjectHandle& handle)
{
    if (!IsValid(handle))
    {
        return;
    }

    Slot& slot = mGameObjectSlots[handle.mIndex];
    slot.generation++;
    mToBeDestroyed.push_back(handle.mIndex);
}

void GameWorld::LoadLevel(const std::filesystem::path levelFile, bool isEditor)
{
    mLevelFileName = levelFile;

    FILE* file = nullptr;
    auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
    ASSERT(err == 0 && file != nullptr, "GameWorld: failed to load level %s", levelFile.u8string().c_str());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);

    auto services = doc["Services"].GetObj();
    for (auto& service : services)
    {
        std::string serviceName = service.name.GetString();
        Service* newService = nullptr;
        if (serviceName == "CameraService")
        {
            newService = AddService<CameraService>();
        }
        else if (serviceName == "RenderService")
        {
            newService = AddService<RenderService>();
        }
        else if (serviceName == "PhysicsService")
        {
            newService = AddService<PhysicsService>();
        }
        else if (serviceName == "UIRenderService")
        {
            newService = AddService<UIRenderService>();
        }
        else
        {
            newService = TryAddService(serviceName, *this); //attempt to add custom service (name, gameworld), if return value, success
            ASSERT(newService != nullptr, "GameWorld: invalid service name %s", serviceName.c_str());
        }

        newService->Deserialize(service.value);
    }

    uint32_t capacity = static_cast<uint32_t>(doc["Capacity"].GetInt());
    Initialize(capacity);

    auto gameObjects = doc["GameObjects"].GetObj();
    for (auto& gameObject : gameObjects)
    {
        std::string name = gameObject.name.GetString();
        std::string templateFile = gameObject.value["Template"].GetString();
        GameObject* obj = CreateGameObject(name, templateFile);
        GameObjectFactory::OverrideDeserialize(gameObject.value, *obj);
        obj->Initialize();
    }

    mEditGameObject = nullptr;
    if (isEditor && !sEditTemplateFile.empty())
    {
        GameObject* obj = CreateGameObject("EditObject", sEditTemplateFile);
        obj->Initialize();
    }
    mIsEditor = isEditor;
}

bool GameWorld::IsInEditMode() const
{
    return mIsEditor;
}

void GameWorld::EditTemplate(const std::filesystem::path& templatePath)
{
    sEditTemplateFile = templatePath;
}

bool GameWorld::IsRequestEdit()
{
    return !sEditTemplateFile.empty();
}

bool GameWorld::IsValid(const GameObjectHandle& handle)
{
    if (handle.mIndex < 0 || handle.mIndex >= mGameObjectSlots.size())
    {
        return false;
    }
    if (mGameObjectSlots[handle.mIndex].generation != handle.mGeneration)
    {
        return false;
    }
    return true;
}

void GameWorld::ProcessDestroyList()
{
    for (uint32_t index : mToBeDestroyed)
    {
        Slot& slot = mGameObjectSlots[index];
        GameObject* gameObject = slot.gameObject.get();
        ASSERT(!IsValid(gameObject->GetHandle()), "GameWorld: object is still alive!");

        gameObject->Terminate();
        slot.gameObject.reset();
        mFreeSlots.push_back(index);
    }
    mToBeDestroyed.clear();
}

void GameWorld::SaveEditTemplate()
{
    if (sEditTemplateFile.empty() || mEditGameObject == nullptr)
    {
        return;
    }

    FILE* file = nullptr;
    auto err = fopen_s(&file, sEditTemplateFile.u8string().c_str(), "r");
    ASSERT(err == 0 && file != nullptr, "GameWorld: failed to open save file %s.", sEditTemplateFile.u8string().c_str());

    char buffer[65536];
    rapidjson::FileReadStream readStream(file, buffer, sizeof(buffer));
    fclose(file);

    rapidjson::Document originalDoc;
    originalDoc.ParseStream(readStream);

    rapidjson::Document writeDoc;
    GameObjectFactory::SerializeGameObject(writeDoc, originalDoc, *mEditGameObject);

    err = fopen_s(&file, sEditTemplateFile.u8string().c_str(), "w");
    ASSERT(err == 0 && file != nullptr, "GameWorld: failed to open save file %s.", sEditTemplateFile.u8string().c_str());
    rapidjson::FileWriteStream writeStream(file, buffer, sizeof(buffer));
    rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
    writeDoc.Accept(writer);
    fclose(file);
}
