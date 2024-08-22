#pragma once
#include "MeshTypes.h"

namespace PanicEngine::Graphics
{
    class MeshBuilder
    {
    public:
        static MeshPX CreateSkyboxPX(float size);
        static MeshPX CreateSkyspherePX(int slices, int rings, float radius);

        static MeshPC CreateCubePC(float size);
        static MeshPX CreateCubePX(float size);

        static MeshPC CreateRectPC(float height, float width, float length);

        static MeshPC CreatePlanePC(int numRows, int numCols, float spacing);
        static MeshPX CreatePlanePX(int numRows, int numCols, float spacing);

        static MeshPC CreateCylinderPC(int slices, int rings);

        static MeshPC CreateSpherePC(int slices, int rings, float radius);
        static MeshPX CreateSpherePX(int slices, int rings, float radius);
    };
}