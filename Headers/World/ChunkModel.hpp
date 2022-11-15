#pragma once

#include "Resources/Mesh.hpp"
#include "Core/Maths/Maths.hpp"
#include "Resources/ShaderProgram.hpp"

namespace World
{
    class ChunkModel
    {
    private:
        Resources::Mesh mesh = Resources::Mesh();
    public:
        Core::Maths::Mat4D modelMat = Core::Maths::Mat4D::Identity();

        ChunkModel();
        ~ChunkModel();

        void Render(Resources::ShaderProgram* shaderProgram, unsigned int& VAOCurrent, const Core::Maths::Mat4D& vp);

        bool IsLoaded() { return mesh.IsLoaded(); }

        friend class Core::Util::ModelLoader;
    };
}