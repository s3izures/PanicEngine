#include "Precompiled.h"
#include "MeshComponent.h"
#include "SaveUtil.h"

using namespace PanicEngine;

void MeshComponent::Deserialize(const rapidjson::Value& value)
{
    RenderObjectComponent::Deserialize(value);

    ASSERT(value.HasMember("Shape") || !mModel.meshData.empty(), "MeshComponent: either needs shape data or has data already");
    Graphics::Model::MeshData& meshData = value.HasMember("Shape") ? mModel.meshData.emplace_back() : mModel.meshData.back();
    Graphics::Model::MaterialData& matData = value.HasMember("Shape") ? mModel.materialData.emplace_back() : mModel.materialData.back();
    if (value.HasMember("Shape"))
    {
        const auto& shapeData = value["Shape"].GetObj();
        if (shapeData.HasMember("Type"))
        {
            std::string shapeType = shapeData["Type"].GetString();
            if (shapeType == "Sphere")
            {
                uint32_t slices = static_cast<uint32_t>(shapeData["Slices"].GetInt());
                uint32_t rings = static_cast<uint32_t>(shapeData["Rings"].GetInt());
                float radius = shapeData["Radius"].GetFloat();
                meshData.mesh = Graphics::MeshBuilder::CreateSphere(slices, rings, radius);
            }
            else if (shapeType == "Plane")
            {
                uint32_t rows = static_cast<uint32_t>(shapeData["Rows"].GetInt());
                uint32_t columns = static_cast<uint32_t>(shapeData["Columns"].GetInt());
                float spacing = shapeData["Spacing"].GetFloat();
                meshData.mesh = Graphics::MeshBuilder::CreateGroundPlane(rows, columns, spacing);
            }
            else if (shapeType == "Cube")
            {
                float size = shapeData["Size"].GetFloat();
                meshData.mesh = Graphics::MeshBuilder::CreateCube(size);
            }
            /*else if (shapeType == "SpriteQuad")
            {
                float width = shapeData["Width"].GetFloat();
                float height = shapeData["Height"].GetFloat();
                meshData.mesh = Graphics::MeshBuilder::CreateSpriteQuad(width, height);
            }*/
            else
            {
                ASSERT(false, "MeshComponent: unrecognized shape type %s", shapeType.c_str());
            }
        }
        else
        {
            ASSERT(false, "MeshComponent: must specify a shape type");
        }
    }
    if (value.HasMember("Material"))
    {
        const auto& materialData = value["Material"].GetObj();
        SaveUtil::ReadColor("Emissive", matData.material.emissive, materialData);
        SaveUtil::ReadColor("Ambient", matData.material.ambient, materialData);
        SaveUtil::ReadColor("Diffuse", matData.material.diffuse, materialData);
        SaveUtil::ReadColor("Specular", matData.material.specular, materialData);
        SaveUtil::ReadFloat("SpecularPower", matData.material.power, materialData);
    }
    if (value.HasMember("Textures"))
    {
        const auto& textureData = value["Textures"].GetObj();
        SaveUtil::ReadString("DiffuseMap", matData.diffuseMapName, textureData);
        SaveUtil::ReadString("NormalMap", matData.normalMapName, textureData);
        SaveUtil::ReadString("SpecMap", matData.specMapName, textureData);
        SaveUtil::ReadString("BumpMap", matData.bumpMapName, textureData);
    }
}

void MeshComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& original)
{
    rapidjson::Value componentValue(rapidjson::kObjectType);
    RenderObjectComponent::Serialize(doc, componentValue, original);

    // shape cannot be changed via the editor
    if (original.HasMember("Shape"))
    {
        rapidjson::Value shapeObject(rapidjson::kObjectType);
        shapeObject.CopyFrom(original["Shape"], doc.GetAllocator());
        componentValue.AddMember("Shape", shapeObject, doc.GetAllocator());
    }
    if (original.HasMember("Material") && !mModel.materialData.empty())
    {
        Graphics::Model::MaterialData& matData = mModel.materialData.front();
        rapidjson::Value materialData(rapidjson::kObjectType);
        SaveUtil::WriteColor("Emissive", matData.material.emissive, doc, materialData);
        SaveUtil::WriteColor("Ambient", matData.material.ambient, doc, materialData);
        SaveUtil::WriteColor("Diffuse", matData.material.diffuse, doc, materialData);
        SaveUtil::WriteColor("Specular", matData.material.specular, doc, materialData);
        SaveUtil::WriteFloat("SpecularPower", matData.material.power, doc, materialData);
        componentValue.AddMember("Material", materialData, doc.GetAllocator());
    }
    if (original.HasMember("Textures") && !mModel.materialData.empty())
    {
        Graphics::Model::MaterialData& matData = mModel.materialData.front();
        rapidjson::Value textureData(rapidjson::kObjectType);
        SaveUtil::WriteString("DiffuseMap", matData.diffuseMapName.c_str(), doc, textureData);
        SaveUtil::WriteString("NormalMap", matData.normalMapName.c_str(), doc, textureData);
        SaveUtil::WriteString("SpecMap", matData.specMapName.c_str(), doc, textureData);
        SaveUtil::WriteString("BumpMap", matData.bumpMapName.c_str(), doc, textureData);
        componentValue.AddMember("Material", textureData, doc.GetAllocator());
    }

    value.AddMember("MeshComponent", componentValue, doc.GetAllocator());
}

const Graphics::Model& MeshComponent::GetModel() const
{
    return mModel;
}
