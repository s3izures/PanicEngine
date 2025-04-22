#pragma once

#include "Service.h"
#include "GameObject.h"

namespace PanicEngine
{
    class GameWorld final
    {
    public:
        void Initialize(uint32_t capacity = 10);
        void Terminate();
        void Update(float deltaTime);
        void Render();
        void DebugUI();

        GameObject* CreateGameObject(std::string name, const std::filesystem::path& templatePath = "");
        void DestroyGameObject(const GameObjectHandle& handle);

        template<class ServiceType>
        ServiceType* AddService()
        {
            static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld: service must be of type Service");
            ASSERT(!mInitialized, "GameWorld: can't add service after initialized");

            auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
            newService->mWorld = this;
            return static_cast<ServiceType*>(newService.get());
        }

        template<class ServiceType>
        ServiceType* GetService() const
        {
            for (auto& service : mServices)
            {
                if (service->GetTypeId() == ServiceType::StaticGetTypeId())
                {
                    return static_cast<ServiceType*>(service.get());
                }
            }
            return nullptr;
        }

        template<class ServiceType>
        ServiceType* GetService()
        {
            const GameWorld* thisConst = static_cast<const GameWorld*>(this);
            return const_cast<ServiceType*>(thisConst->GetService<ServiceType>());
        }

    private:
        bool IsValid(const GameObjectHandle& handle);
        void ProcessDestroyList();

        struct Slot
        {
            std::unique_ptr<GameObject> gameObject;
            uint32_t generation = 0;
        };

        using GameObjectSlots = std::vector<Slot>;
        GameObjectSlots mGameObjectSlots;
        std::vector<uint32_t> mFreeSlots;
        std::vector<uint32_t> mToBeDestroyed;

        using Services = std::vector<std::unique_ptr<Service>>;
        Services mServices;

        bool mInitialized = false;
    };
}