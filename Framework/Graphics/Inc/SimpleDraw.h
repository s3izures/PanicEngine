#pragma once

#include "Color.h"

namespace PanicEngine::Graphics
{
    class Camera;

    namespace SimpleDraw
    {
        void StaticInitialize(uint32_t maxVertexCount);
        void StaticTerminate();

        void AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color);
        void AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color);

        void AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
        void AddAABB(float minX, float minY, float minz, float maxX, float maxY, float maxZ, const Color& color);

        void AddFilledAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color);
        void AddFilledAABB(float minX, float minY, float minz, float maxX, float maxY, float maxZ, const Color& color);

        void AddSphere(int slices, int rings, float radius, const Math::Vector3& pos, const Color& color);
        void AddGroundCircle(int slices, float radius, const Math::Vector3& pos, const Color& color);

        void AddGroundPlane(float size, const Color& color);

        void AddTransform(const Math::Matrix4& m);

        void Render(const Camera& camera);
    }
}