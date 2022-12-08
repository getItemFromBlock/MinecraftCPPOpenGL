#pragma once

#include "Resources/Mesh.hpp"
#include "Core/Maths/Maths.hpp"
#include "Resources/ShaderProgram.hpp"
#include "Resources/Material.hpp"

#include <vector>

namespace Resources
{
    class ModelHolder;
    class ShaderManager;
    class MeshManager;
    class ResourceManager;
    class MaterialManager;
    class TextureManager;
}

namespace Core::Util
{
    class ModelLoader;
}

namespace LowRenderer
{
    namespace Rendering
    {
        class RenderCamera;
    }
    namespace Lightning
    {
        class LightManager;
    }

    class Model
    {
    private:
        std::vector<Resources::Mesh*> models = std::vector<Resources::Mesh*>();
        std::vector<Resources::Material> materials = std::vector<Resources::Material>();
        Resources::Texture* currentTexture = nullptr;
        Resources::Texture* currentNormalMap = nullptr;
    public:
        Core::Maths::Mat4 modelMat = Core::Maths::Mat4::Identity();
        Resources::ShaderProgram* shaderProgram = nullptr;

        Model();
        Model(Resources::ShaderProgram* Shader);
        ~Model();

        void Render(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& vp, const std::vector<Core::Maths::Mat4>* lvp);

        void AddMesh(Resources::Mesh* modelIn, Resources::ResourceManager* manager, Resources::Material* modelMaterial);

        void CreateFrom(const Model* other);

        void DeleteModel();

        void Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime);

        int GetNumberOfTriangles();

        void SetColor(Core::Maths::Vec3 newColor);

        void SetTexture(unsigned int textureID);

        void LoadMesh(const char* path, Resources::ResourceManager* manager, Resources::MaterialManager* materialManager, Resources::TextureManager* textureManager, Resources::MeshManager* modelManager);

        friend class Core::Util::ModelLoader;
    };
}