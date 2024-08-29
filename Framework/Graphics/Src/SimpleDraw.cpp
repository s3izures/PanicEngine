#include "Precompiled.h"
#include "SimpleDraw.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "MeshBuffer.h"
#include "Camera.h"
#include "VertexTypes.h"
#include "BlendState.h"

using namespace PanicEngine;
using namespace PanicEngine::Graphics;
using namespace PanicEngine::Math;
using namespace PanicEngine::Math::Constants;

namespace
{
    //singleton accessible only to SimpleDraw

    class SimpleDrawImpl
    {
    public:
        void Initialize(uint32_t maxVertexCount);
        void Terminate();

        void AddLine(const Vector3& v0, const Vector3& v1, const Color& color);
        void AddLine(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color);

        void Render(const Camera& camera);

    private:
        VertexShader mVertexShader;
        PixelShader mPixelShader;
        ConstantBuffer mConstantBuffer;
        MeshBuffer mMeshBuffer;
        BlendState mBlendState;
        
        std::unique_ptr<VertexPC[]> mLineVertices;
        std::unique_ptr<VertexPC[]> mFaceVertices;

        uint32_t mLineVertexCount = 0;
        uint32_t mFaceVertexCount = 0;
        uint32_t mMaxVertexCount = 0;
    };

    void SimpleDrawImpl::Initialize(uint32_t maxVertexCount)
    {
        std::filesystem::path shaderFile = "../../Assets/Shaders/DoTransform.fx";
        mVertexShader.Initialize<VertexPC>(shaderFile);
        mPixelShader.Initialize(shaderFile);
        mConstantBuffer.Initialize(sizeof(Matrix4));
        mMeshBuffer.Initialize(nullptr, sizeof(VertexPC), maxVertexCount);
        mBlendState.Initialize(BlendState::Mode::AlphaBlend);

        mLineVertices = std::make_unique<VertexPC[]>(maxVertexCount);
        mFaceVertices = std::make_unique<VertexPC[]>(maxVertexCount);
        mLineVertexCount = 0;
        mFaceVertexCount = 0;
        mMaxVertexCount = maxVertexCount;
    }
    void SimpleDrawImpl::Terminate()
    {
        mBlendState.Terminate();
        mMeshBuffer.Terminate();
        mConstantBuffer.Terminate();
        mPixelShader.Terminate();
        mVertexShader.Terminate();
    }
    void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Color& color)
    {
        if (mLineVertexCount + 2 <= mMaxVertexCount)
        {
            mLineVertices[mLineVertexCount++] = VertexPC{ v0, color };
            mLineVertices[mLineVertexCount++] = VertexPC{ v1, color };
        }
    }
    void SimpleDrawImpl::AddLine(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color)
    {
        if (mLineVertexCount + 3 <= mMaxVertexCount)
        {
            mFaceVertices[mFaceVertexCount++] = VertexPC{ v0, color };
            mFaceVertices[mFaceVertexCount++] = VertexPC{ v1, color };
            mFaceVertices[mFaceVertexCount++] = VertexPC{ v2, color };
        }
    }
    void SimpleDrawImpl::Render(const Camera& camera)
    {
        const Matrix4 matView = camera.GetViewMatrix();
        const Matrix4 matProj = camera.GetProjectionMatrix();
        const Matrix4 transform = Transpose(matView * matProj);
        mConstantBuffer.Update(&transform);
        mConstantBuffer.BindVS(0);

        mVertexShader.Bind();
        mPixelShader.Bind();

        mBlendState.Set();

        mMeshBuffer.Update(mLineVertices.get(), mLineVertexCount);
        mMeshBuffer.SetTopology(MeshBuffer::Topology::Lines);
        mMeshBuffer.Render();

        mMeshBuffer.Update(mFaceVertices.get(), mFaceVertexCount);
        mMeshBuffer.SetTopology(MeshBuffer::Topology::Triangles);
        mMeshBuffer.Render();

        mBlendState.ClearState();

        mLineVertexCount = 0;
        mFaceVertexCount = 0;
    }

    std::unique_ptr<SimpleDrawImpl> sInstance;
}

void SimpleDraw::StaticInitialize(uint32_t maxVertexCount)
{
    sInstance = std::make_unique<SimpleDrawImpl>();
    sInstance->Initialize(maxVertexCount);
}

void SimpleDraw::StaticTerminate()
{
    sInstance->Terminate();
    sInstance.reset();
}

void SimpleDraw::AddLine(const Math::Vector3& v0, const Math::Vector3& v1, const Color& color)
{
    sInstance->AddLine(v0, v1, color);
}

void SimpleDraw::AddFace(const Math::Vector3& v0, const Math::Vector3& v1, const Math::Vector3& v2, const Color& color)
{
    sInstance->AddLine(v0, v1, v2, color);
}

void SimpleDraw::AddAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
    AddAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}

void SimpleDraw::AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
    const Vector3 blf = { minX,minY,minZ };
    const Vector3 tlf = { minX,maxY,minZ };
    const Vector3 trf = { maxX,maxY,minZ };
    const Vector3 brf = { maxX,minY,minZ };

    const Vector3 blb = { minX,minY,maxZ };
    const Vector3 tlb = { minX,maxY,maxZ };
    const Vector3 trb = { maxX,maxY,maxZ };
    const Vector3 brb = { maxX,minY,maxZ };

    //front
    AddLine(blf, tlf, color);
    AddLine(tlf, trf, color);
    AddLine(trf, brf, color);
    AddLine(brf, blf, color);

    //back
    AddLine(blb, tlb, color);
    AddLine(tlb, trb, color);
    AddLine(trb, brb, color);
    AddLine(brb, blb, color);

    //top
    AddLine(tlf, tlb, color);
    AddLine(trf, trb, color);

    //bottom
    AddLine(blf, blb, color);
    AddLine(brf, brb, color);
}

void SimpleDraw::AddFilledAABB(const Math::Vector3& min, const Math::Vector3& max, const Color& color)
{
    AddFilledAABB(min.x, min.y, min.z, max.x, max.y, max.z, color);
}

void SimpleDraw::AddFilledAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Color& color)
{
    const Vector3 blf = { minX,minY,minZ };
    const Vector3 tlf = { minX,maxY,minZ };
    const Vector3 trf = { maxX,maxY,minZ };
    const Vector3 brf = { maxX,minY,minZ };

    const Vector3 blb = { minX,minY,maxZ };
    const Vector3 tlb = { minX,maxY,maxZ };
    const Vector3 trb = { maxX,maxY,maxZ };
    const Vector3 brb = { maxX,minY,maxZ };

    //front
    AddFace(blf, tlf, trf, color);
    AddFace(blf, trf, brf, color);

    //back
    AddFace(brb, trb, tlb, color);
    AddFace(brb, tlb, blb, color);

    //right
    AddFace(brf, trf, trb, color);
    AddFace(brf, trb, brb, color);

    //left
    AddFace(blb, tlb, tlf, color);
    AddFace(blb, tlf, blf, color);

    //top
    AddFace(tlf, tlb, trb, color);
    AddFace(tlf, trb, trf, color);

    //bottom
    AddFace(brf, brb, blb, color);
    AddFace(brf, blb, blf, color);
}

void SimpleDraw::AddSphere(int slices, int rings, float radius, const Math::Vector3& pos, const Color& color)
{
    Vector3 v0 = Vector3::Zero;
    Vector3 v1 = Vector3::Zero;

    const float vertRot = (Pi / static_cast<float>(rings));
    const float horzRot = (TwoPi / static_cast<float>(slices));
    for (int r = 0; r < rings; ++r)
    {
        int ring = static_cast<float>(r);
        float phi = ring * vertRot;
        for (int s = 0; s < slices; ++s)
        {
            float slice0 = static_cast<float>(s);
            float rot0 = slice0 * horzRot;

            float slice1 = static_cast<float>(s + 1);
            float rot1 = slice1 * horzRot;

            v0 =
            {
                sin(rot0) * sin(phi) * radius,
                cos(phi) * radius,
                cos(rot0) * sin(phi) * radius
            };

            v1 =
            {
                sin(rot1) * sin(phi) * radius,
                cos(phi) * radius,
                cos(rot1) * sin(phi) * radius
            };
            AddLine(v0 + pos, v1 + pos, color);

            v0 =
            {
                cos(phi) * radius,
                cos(rot0) * sin(phi) * radius,
                sin(rot0) * sin(phi) * radius
            };

            v1 =
            {
                cos(phi) * radius,
                cos(rot1) * sin(phi) * radius,
                sin(rot1) * sin(phi) * radius
            };
            AddLine(v0 + pos, v1 + pos, color);
        }
    }
}

void SimpleDraw::AddGroundCircle(int slices, float radius, const Math::Vector3& pos, const Color& color)
{
    Vector3 v0 = Vector3::Zero;
    Vector3 v1 = Vector3::Zero;

    float horRot = (TwoPi / static_cast<float>(slices));
    for (int s = 0; s < slices; ++s)
    {
        float slice0 = static_cast<float>(s);
        float rot0 = slice0 * horRot;

        float slice1 = static_cast<float>(s + 1);
        float rot1 = slice1 * horRot;

        v0 =
        {
            sin(rot0) * radius,
            0.0f,
            cos(rot0) * radius
        };

        v1 =
        {
            sin(rot1) * radius,
            0.0f,
            cos(rot1) * radius
        };

        AddLine(v0 + pos, v1 + pos, color);
    }
}

void SimpleDraw::AddGroundPlane(float size, const Color& color)
{
    const float hs = size * 0.5f;
    for (int i = 0; i <= size; ++i)
    {
        AddLine({ i - hs, 0.0f, -hs }, { i - hs, 0.0f, hs }, color);
        AddLine({ -hs, 0.0f, i - hs }, { hs, 0.0f, i - hs }, color);
    }
}

void SimpleDraw::AddTransform(const Math::Matrix4& m)
{
    const Vector3 side = { m._11, m._12, m._13 };
    const Vector3 up = { m._21, m._22, m._23 };
    const Vector3 look = { m._31, m._32, m._33 };
    const Vector3 pos = { m._41, m._42, m._43 };

    AddLine(pos, pos + side, Colors::Red);
    AddLine(pos, pos + up, Colors::Blue);
    AddLine(pos, pos + look, Colors::Green);
}

void SimpleDraw::Render(const Camera& camera)
{
    sInstance->Render(camera);
}
