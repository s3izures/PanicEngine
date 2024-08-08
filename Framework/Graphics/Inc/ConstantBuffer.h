#pragma once

namespace PanicEngine::Graphics
{
    class ConstantBuffer
    {
    public:
        ConstantBuffer() = default;
        virtual ~ConstantBuffer();

        void Initialize(uint32_t bufferSize);
        void Terminate();

        void Update(const void* data) const;

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

    private:
        ID3D11Buffer* mConstantBuffer = nullptr;
    };
}