#pragma once
#include "MeshTypes.h"

namespace PanicEngine::Graphics
{
    class Terrain final
    {
    public:
        void Initialize(const std::filesystem::path& filename, float maxHeight, float tileCount);
        float GetHeight(const Math::Vector3& position) const;
        const Mesh& GetMesh() const;

        float GetWidth() const;
        float GetHeight() const;

    private:
        Mesh mMesh;
        uint32_t mRows = 0;
        uint32_t mColumns = 0;
    };
}