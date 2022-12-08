#include "LowRenderer/Lightning/LightManager.hpp"

#include <glad/glad.h>

#include "LowRenderer/Lightning/Light.hpp"

LowRenderer::Lightning::LightManager::LightManager()
{
}

LowRenderer::Lightning::LightManager::~LightManager()
{
}

void LowRenderer::Lightning::LightManager::WriteLights(Resources::ShaderProgram** shaderProgramCurrent, Resources::ShaderProgram* shader, const Core::Maths::Vec3& cameraPos, std::vector< Core::Maths::Mat4>* lvp, bool first, float GlobalTime)
{
	
}

void LowRenderer::Lightning::LightManager::EmptyLights()
{
	DLights.clear();
	PLights.clear();
	SLights.clear();
	ShadowMapLights.clear();
}

void LowRenderer::Lightning::LightManager::UpdateShaders(Resources::ShaderProgram** shaderProgramCurrent, Resources::ShaderManager* manager, const Core::Maths::Vec3& cameraPos, std::vector<Core::Maths::Mat4>* lvp, float GlobalTime)
{
	std::vector<Resources::ShaderProgram*> shaders = manager->GetShaderPrograms();
	for (unsigned int i = 0; i < shaders.size(); i++)
	{
		WriteLights(shaderProgramCurrent, shaders[i], cameraPos, lvp, i==0, GlobalTime);
	}
}

void LowRenderer::Lightning::LightManager::Init(Resources::ResourceManager* resources)
{
	ShadowMapBuffer::SetShadowMapResolution(1);
	defaultBuffer = resources->Create<ShadowMapBuffer>("Default Shadow Map Buffer");
	ShadowMapBuffer::SetShadowMapResolution(2048);
}