#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
            Colors::Aqua,
            Colors::Beige,
            Colors::Violet,
            Colors::HotPink,
            Colors::Magenta,
            Colors::LightGoldenrodYellow,
            Colors::Lime,
            Colors::Teal
        };

        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }

    void CreateCubeIndices(std::vector<uint32_t>& indices)
    {
        indices = {
            // front
            0, 1, 2,
            0, 2, 3,

            // back
            7, 6, 5,
            7, 5, 4,

            // right
            3, 2, 6,
            3, 6, 7,

            // left
            0, 5, 1,
            0, 4, 5,

            // top
            1, 5, 6,
            1, 6, 2,

            // bottom
            0, 3, 7,
            0, 7, 4
        };
    }

    void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numCols)
    {
        for (int r = 0; r < numRows; ++r)
        {
            for (int c = 0; c < numCols; ++c)
            {
                int i = (r * (numCols + 1)) + c;

                // triangle 0
                indices.push_back(i);
                indices.push_back(i + numCols + 2);
                indices.push_back(i + 1);

                // triangle 1
                indices.push_back(i);
                indices.push_back(i + numCols + 1);
                indices.push_back(i + numCols + 2);
            }
        }
    }

    void CreateCapIndices(std::vector<uint32_t>& indices, int slices, int topIndex, int bottomIndex)
    {
        for (int s = 0; s < slices; ++s)
        {
            // bottom triangle
            indices.push_back(bottomIndex);
            indices.push_back(s);
            indices.push_back(s + 1);

            // top triangle
            int topRowIndex = topIndex - slices - 1 + s;
            indices.push_back(topIndex);
            indices.push_back(topRowIndex + 1);
            indices.push_back(topRowIndex);
        }
    }
}

MeshPX MeshBuilder::CreateSkyboxPX(float size)
{
    MeshPX mesh;

    const float hs = size * 0.5f;    // half size
    const float ot = 1.0f / 3.0f;    // one third
    const float tt = 2.0f / 3.0f;    // two third

    // front
    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, { 0.5f, ot} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });

    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, { 0.5f, ot} });

    // right
    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {0.75f, ot} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, { 0.5f, ot} });

    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {0.75f, ot} });

    // back
    mesh.vertices.push_back({ { hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 1.0f, ot} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {0.75f, ot} });

    mesh.vertices.push_back({ { hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ {-hs, -hs,  hs}, { 1.0f, tt} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 1.0f, ot} });

    // left
    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 0.0f, ot} });

    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 0.0f, ot} });
    mesh.vertices.push_back({ {-hs,  -hs, hs}, { 0.0f, tt} });

    // top
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, { 0.5f, 0.0f} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, {0.25f, 0.0f} });

    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ { hs,  hs,  -hs}, { 0.5f, ot} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, { 0.5f, 0.0f} });


    // bottom
    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {-hs, -hs,  hs}, {0.25f, 1.0f} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, { 0.5f, 1.0f} });

    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, { 0.5f, 1.0f} });
    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });

    for (uint32_t i = 0; i < mesh.vertices.size(); ++i)
    {
        mesh.indices.push_back(i);
    }

    return mesh;
}

MeshPX MeshBuilder::CreateSkyspherePX(int slices, int rings, float radius)
{
    MeshPX mesh;

    float verRot = (Math::Constants::Pi / static_cast<float>(rings - 1));
    float horRot = (Math::Constants::TwoPi / static_cast<float>(slices));
    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        float phi = ring * verRot;
        for (int s = 0; s <= slices; ++s)
        {
            float slice = static_cast<float>(s);
            float rot = slice * horRot;

            float u = 1.0f - (uStep * slice);
            float v = vStep * ring;

            mesh.vertices.push_back({ {
                radius * cos(rot) * sin(phi),
                radius * cos(phi),
                radius * sin(rot) * sin(phi) },
                {u, v} });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);
    return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    MeshPC mesh;

    int index = rand() % 10;


    const float hs = size * 0.5f;	// half size

    // front
    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

    // back
    mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColor(index) });


    // indices
    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPX MeshBuilder::CreateCubePX(float size)
{
    MeshPX mesh;

    const float hs = size * 0.5f;    // half size
    const float ot = 1.0f / 3.0f;    // one third
    const float tt = 2.0f / 3.0f;    // two third

    // front
    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, { 0.5f, ot} });

    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, { 0.5f, ot} });
    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });

    // right
    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, { 0.5f, ot} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {0.75f, ot} });

    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {0.75f, ot} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, {0.75f, tt} });

    // back
    mesh.vertices.push_back({ { hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {0.75f, ot} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 1.0f, ot} });

    mesh.vertices.push_back({ { hs, -hs,  hs}, {0.75f, tt} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 1.0f, ot} });
    mesh.vertices.push_back({ {-hs, -hs,  hs}, { 1.0f, tt} });

    // left
    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 0.0f, ot} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });

    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ {-hs,  -hs, hs}, { 0.0f, tt} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, { 0.0f, ot} });

    // top
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, {0.25f, 0.0f} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, { 0.5f, 0.0f} });

    mesh.vertices.push_back({ {-hs,  hs, -hs}, {0.25f, ot} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, { 0.5f, 0.0f} });
    mesh.vertices.push_back({ { hs,  hs,  -hs}, { 0.5f, ot} });


    // bottom
    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, { 0.5f, 1.0f} });
    mesh.vertices.push_back({ {-hs, -hs,  hs}, {0.25f, 1.0f} });

    mesh.vertices.push_back({ {-hs, -hs, -hs}, {0.25f, tt} });
    mesh.vertices.push_back({ { hs, -hs, -hs}, { 0.5f, tt} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, { 0.5f, 1.0f} });

    for (uint32_t i = 0; i < mesh.vertices.size(); ++i)
    {
        mesh.indices.push_back(i);
    }

    return mesh;
}

MeshPC MeshBuilder::CreateRectPC(float height, float width, float length)
{
    MeshPC mesh;

    int index = rand() % 10;

    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hl = length * 0.5f;

    // front
    mesh.vertices.push_back({ { -hw, -hh, -hl }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, -hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, -hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, -hl }, GetNextColor(index) });

    // back
    mesh.vertices.push_back({ { -hw, -hh, hl }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, hl }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, hl }, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(int numRows, int numCols, float spacing)
{
    MeshPC mesh;
    int index = rand() % 10;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float x = -hpw;
    float y = -hph;

    for (int r = 0; r <= numRows; ++r)
    {
        for (int c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({{ x,y,0.0f }, GetNextColor(index)});
            x += spacing;
        }
        x = -hpw;
        y += spacing;
    }
    CreatePlaneIndices(mesh.indices, numRows, numCols);
    return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(int numRows, int numCols, float spacing)
{
    MeshPX mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float x = -hpw;
    float y = -hph;
    float uInc = 1.0f / static_cast<float>(numCols);
    float vInc = -1.0f / static_cast<float>(numRows);
    float u = 0.0f;
    float v = 1.0f;

    for (int r = 0; r <= numRows; ++r)
    {
        for (int c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({ { x,y,0.0f }, {u, v} });
            x += spacing;
            u += uInc;
        }
        x = -hpw;
        y += spacing;
        u = 0.0f;
        v += vInc;
    }
    CreatePlaneIndices(mesh.indices, numRows, numCols);
    return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(int slices, int rings)
{
    MeshPC mesh;
    int index = rand() % 10;

    const float hh = static_cast<float>(rings) * 0.5f;

    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        for (int s = 0; s <= slices; ++s)
        {
            float slice = static_cast<float>(s);
            float rotation = (slice / static_cast<float>(slices)) * Math::Constants::TwoPi;

            mesh.vertices.push_back({ {
                    sin(rotation),
                    ring - hh,
                    -cos(rotation)},
                    GetNextColor(index) });
        }
    }

    mesh.vertices.push_back({ {0.0f,  hh, 0.0f}, GetNextColor(index) });
    mesh.vertices.push_back({ {0.0f, -hh, 0.0f}, GetNextColor(index) });

    CreatePlaneIndices(mesh.indices, rings, slices);
    CreateCapIndices(mesh.indices, slices, mesh.vertices.size() - 2, mesh.vertices.size() - 1);
    return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
    MeshPC mesh;
    int index = rand() % 10;

    float verRot = (Math::Constants::Pi / static_cast<float>(rings - 1));
    float horRot = (Math::Constants::TwoPi / static_cast<float>(slices));
    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        float phi = ring * verRot;
        for (int s = 0; s <= slices; ++s)
        {
            float slice = static_cast<float>(s);
            float rot = slice * horRot;
            mesh.vertices.push_back({ {
                radius * sin(rot) * sin(phi),
                radius * cos(phi),
                radius * cos(rot) * sin(phi) },
                GetNextColor(index) });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);
    return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
    MeshPX mesh;

    float verRot = (Math::Constants::Pi / static_cast<float>(rings - 1));
    float horRot = (Math::Constants::TwoPi / static_cast<float>(slices));
    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        float phi = ring * verRot;
        for (int s = 0; s <= slices; ++s)
        {
            float slice = static_cast<float>(s);
            float rot = slice * horRot;

            float u = 1.0f - (uStep * slice);
            float v = vStep * ring;

            mesh.vertices.push_back({ {
                radius * sin(rot) * sin(phi),
                radius * cos(phi),
                radius * cos(rot) * sin(phi) },
                {u, v} });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);
    return mesh;
}

Mesh MeshBuilder::CreateSphere(int slices, int rings, float radius)
{
    Mesh mesh;

    float verRot = (Math::Constants::Pi / static_cast<float>(rings - 1));
    float horRot = (Math::Constants::TwoPi / static_cast<float>(slices));
    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        float phi = ring * verRot;
        for (int s = 0; s <= slices; ++s)
        {
            float slice = static_cast<float>(s);
            float rot = slice * horRot;

            float u = 1.0f - (uStep * slice);
            float v = vStep * ring;

            float x = radius * sin(rot) * sin(phi);
            float y = radius * cos(phi);
            float z = radius * cos(rot) * sin(phi);

            const Math::Vector3 pos = { x,y,z };
            const Math::Vector3 norm = Math::Normalize(pos);
            const Math::Vector3 tang = abs(Math::Dot(norm, Math::Vector3::YAxis)) < 0.999f ? Math::Normalize({ -z,0.0f,x }) : Math::Vector3::ZAxis;

            mesh.vertices.push_back({ pos,norm,tang,{u,v} });
        }
    }
    CreatePlaneIndices(mesh.indices, rings, slices);
    return mesh;
}
