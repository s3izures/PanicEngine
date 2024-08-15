#pragma once

namespace PanicEngine::Graphics
{
    class Texture
    {
    public:
        static void UnbindPS(uint32_t slot);

        Texture() = default;
        virtual ~Texture();

        //delete copies
        Texture(const Texture&) = delete;
        Texture* operator=(const Texture&) = delete;

        //allow move
        Texture(Texture&& rhs) noexcept;
        Texture& operator=(Texture&& rhs) noexcept;

        virtual void Initialize(const std::filesystem::path& fileName);
        virtual void Terminate();

        void BindVS(uint32_t slot) const;
        void BindPS(uint32_t slot) const;

        void* GetRawData() const;

    private:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;
    };
}
