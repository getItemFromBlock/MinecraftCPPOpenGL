#include "LowRenderer/Model.hpp"

#include <glad/glad.h>
#include <ImGUI/imgui.h>

#include "LowRenderer/Lightning/LightManager.hpp"
#include "Resources/ShaderManager.hpp"
#include "Resources/MaterialManager.hpp"
#include "Resources/MeshManager.hpp"
#include "Core/Util/ModelLoader.hpp"
#include "Core/Util/TextHelper.hpp"
#include "Core/App/App.hpp"

LowRenderer::Model::Model()
{
}

LowRenderer::Model::Model(Resources::ShaderProgram* Shader) : shaderProgram(Shader)
{
}

LowRenderer::Model::~Model() {}

void LowRenderer::Model::Render(unsigned int& VAOCurrent, Resources::ShaderProgram** shaderProgramCurrent, const Core::Maths::Mat4& vp, const std::vector<Core::Maths::Mat4>* lvp)
{
	if (!shaderProgram || models.size() == 0 || materials.size() == 0) return;

	// Set matrix of model.
	Core::Maths::Mat4 mvp = vp * modelMat;
	if (*shaderProgramCurrent != shaderProgram)
	{
		glUseProgram(shaderProgram->GetProgramID());
		*shaderProgramCurrent = shaderProgram;
	}
	currentTexture = materials[0].GetTexture();
	currentNormalMap = materials[0].GetNormalMap();
	glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::MVP), 1, GL_FALSE, mvp.content);
	glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::M), 1, GL_FALSE, modelMat.content);
	glUniform1i(shaderProgram->GetLocation(Resources::ShaderData::Texture), currentTexture->GetTextureID());
	if (currentNormalMap) glUniform1i(shaderProgram->GetLocation(Resources::ShaderData::NTexture), currentNormalMap->GetTextureID());
	if (lvp && lvp->size() > 0)
	{
		glUniformMatrix4fv(shaderProgram->GetLocation(Resources::ShaderData::LightMVP), (int)lvp->size(), GL_FALSE, lvp->data()->content);
	}
	// Render all meshs.
	for (size_t i = 0; i < models.size(); i++)
	{
		size_t matIndex = i < materials.size() ? i : materials.size() - 1;
		glUniform3f(shaderProgram->GetLocation(Resources::ShaderData::MatAmbient), materials[matIndex].AmbientColor.x, materials[matIndex].AmbientColor.y, materials[matIndex].AmbientColor.z);
		glUniform3f(shaderProgram->GetLocation(Resources::ShaderData::MatDiffuse), materials[matIndex].DiffuseColor.x, materials[matIndex].DiffuseColor.y, materials[matIndex].DiffuseColor.z);
		glUniform3f(shaderProgram->GetLocation(Resources::ShaderData::MatSpecular), materials[matIndex].SpecularColor.x, materials[matIndex].SpecularColor.y, materials[matIndex].SpecularColor.z);
		glUniform3f(shaderProgram->GetLocation(Resources::ShaderData::MatEmissive), materials[matIndex].EmissiveColor.x, materials[matIndex].EmissiveColor.y, materials[matIndex].EmissiveColor.z);
		glUniform1f(shaderProgram->GetLocation(Resources::ShaderData::MatAlpha), materials[matIndex].Alpha);
		glUniform1f(shaderProgram->GetLocation(Resources::ShaderData::MatSmoothness), materials[matIndex].Smoothness);
		glUniform1f(shaderProgram->GetLocation(Resources::ShaderData::MatShininess), materials[matIndex].Shininess);
		glUniform1f(shaderProgram->GetLocation(Resources::ShaderData::MatAbsorbtion), materials[matIndex].Absorbtion);
		if (materials[matIndex].GetTexture() != currentTexture)
		{
			currentTexture = materials[matIndex].GetTexture();
			glUniform1i(shaderProgram->GetLocation(Resources::ShaderData::Texture), currentTexture->GetTextureID());
		}
		if (materials[matIndex].GetNormalMap() != currentNormalMap)
		{
			currentNormalMap = materials[matIndex].GetNormalMap();
			if (currentNormalMap) glUniform1i(shaderProgram->GetLocation(Resources::ShaderData::NTexture), currentNormalMap->GetTextureID());
		}
		models[i]->Draw(VAOCurrent);
	}
}

int LowRenderer::Model::GetNumberOfTriangles()
{
	int x = 0;
	for (size_t i = 0; i < models.size(); i++)
	{
		x += models[i]->GetVerticesCount() / 3;
	}
	return x;
}

void LowRenderer::Model::SetColor(Core::Maths::Vec3 newColor)
{
	for (size_t i = 0; i < materials.size(); i++) materials[i].AmbientColor = newColor;
}

void LowRenderer::Model::SetTexture(unsigned int textureID)
{
	for (size_t i = 0; i < materials.size(); i++) materials[i].GetTexture()->SetTextureID(textureID);
}

void LowRenderer::Model::LoadMesh(const char* path, Resources::ResourceManager* manager, Resources::MaterialManager* materialManager, Resources::TextureManager* textureManager, Resources::MeshManager* meshManager)
{
	Core::Util::ModelLoader::LoadMesh(*this, path, manager, materialManager, textureManager, meshManager);
	currentTexture = materials.at(0).GetTexture();
}

void LowRenderer::Model::AddMesh(Resources::Mesh* modelIn, Resources::ResourceManager* manager, Resources::Material* modelMaterial)
{
	models.push_back(modelIn);
	if (modelMaterial->GetTexture() == nullptr) modelMaterial->SetTexture(manager->Get<Resources::Texture>("DefaultResources/Textures/Blank.png"));
	if (modelMaterial->GetNormalMap() == nullptr) modelMaterial->SetNormalMap(manager->Get<Resources::Texture>("DefaultResources/Textures/normal.png"));
	//materials.push_back(modelMaterial);
}

void LowRenderer::Model::CreateFrom(const Model* other)
{
	for (size_t i = 0; i < other->models.size(); i++)
	{
		models.push_back(other->models[i]);
	}
	for (size_t i = 0; i < other->materials.size(); i++)
	{
		materials.push_back(other->materials[i]);
	}
}

void LowRenderer::Model::DeleteModel()
{
	for (size_t i = 0; i < materials.size(); i++) materials[i].UnLoad();
	materials.clear();
	this->~Model();
}

void LowRenderer::Model::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime)
{
	modelMat = Core::Maths::Mat4::Identity();
	for (size_t i = 0; i < materials.size(); i++)
	{
		materials[i].Update(resources);
	}
}