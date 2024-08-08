#pragma once

namespace PanicEngine::Graphics
{
    class VertexShader final
    {
    public:
        void Initialize(const std::filesystem::path& filePath, uint32_t format);
        void Terminate();

        void Bind();

    private:
        ID3D11VertexShader* mVertexShader = nullptr;
        ID3D11InputLayout* mInputLayout = nullptr;
    };
}