#include "Precompiled.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "SaveUtil.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

void TransformComponent::DebugUI()
{
    ImGui::DragFloat3("Position", &position.x, 0.1f);
    ImGui::DragFloat4("Rotation", &rotation.x, 0.0001f);
    ImGui::DragFloat3("Scale", &scale.x, 0.1f);

    SimpleDraw::AddTransform(GetMatrix4());
}

void TransformComponent::Deserialize(const rapidjson::Value& value)
{
    SaveUtil::ReadVector3("Position", position, value);
    SaveUtil::ReadQuaternion("Rotation", rotation, value);
    SaveUtil::ReadVector3("Scale", scale, value);
}

void TransformComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    SaveUtil::WriteVector3("Position", position, doc, componentValue);
    SaveUtil::WriteQuaternion("Rotation", rotation, doc, componentValue);
    SaveUtil::WriteVector3("Scale", scale, doc, componentValue);
    value.AddMember("TransformComponent", componentValue, doc.GetAllocator());
}

Transform TransformComponent::GetWorldTransform() const
{
    Transform worldTransform = *this;
    const GameObject* parent = GetOwner().GetParent();
    if (parent != nullptr)
    {
        Math::Matrix4 matWorld = GetMatrix4();
        while (parent != nullptr)
        {
            const TransformComponent* transformComponent = parent->GetComponent<TransformComponent>();
            ASSERT(transformComponent != nullptr, "TransformComponent: parent does not have a transform");
            matWorld = matWorld * transformComponent->GetMatrix4();
            parent = parent->GetParent();
        }
        worldTransform.position = Math::GetTranslation(matWorld);
        worldTransform.rotation = Math::Quaternion::CreateFromRotationMatrix(matWorld);
        worldTransform.scale = Math::GetScale(matWorld);
    }
    return worldTransform;
}