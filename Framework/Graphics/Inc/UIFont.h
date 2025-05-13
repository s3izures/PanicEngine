#pragma once
#include "Color.h"

namespace PanicEngine::Graphics
{
    class UIFont final
    {
    public:
        enum class FontType
        {
            Arial,
            CourierNew,
            Consolas,
            TimesNewRoman,
            Verdana
        };

        static void StaticInitialize(FontType font);
        static void StaticTerminate();
        static UIFont* Get();

        UIFont() = default;
        ~UIFont();

        void Initialize(FontType font);
        void Terminate();

        void DrawString(const wchar_t* str, const Math::Vector3& position, const Color& color, float size = 10);
        float GetStringWidth (const wchar_t* str, float size) const;

    private:
        FontType mFontType = FontType::Arial;
        IFW1Factory* mFontFactory = nullptr;
        IFW1FontWrapper* mFontWrapper = nullptr;
    };
}