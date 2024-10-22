#pragma once

namespace PanicEngine::Graphics
{
    struct Model;

    namespace ModelIO
    {
        void SaveModel(std::filesystem::path filePath, const Model& model);
        void LoadModel(std::filesystem::path filePath, Model& model);
    }
}