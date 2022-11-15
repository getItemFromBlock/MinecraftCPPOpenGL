#include "LowRenderer/Lightning/Light.hpp"

#include <ImGUI/imgui.h>
#include <glad/glad.h>

#include "LowRenderer/Lightning/LightManager.hpp"
#include "Core/Util/TextHelper.hpp"
#include "LowRenderer/Rendering/RenderCamera.hpp"
#include "Resources/MaterialManager.hpp"
#include "Resources/MeshManager.hpp"

void LowRenderer::Light::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime)
{
}

void LowRenderer::DirectionalLight::DeleteLight()
{
	Light::~Light();
	this->~DirectionalLight();
}

void LowRenderer::DirectionalLight::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime)
{
	GlobalDirection = (Core::Maths::Mat4D::CreateTransformMatrix(Core::Maths::Vec3D(), Rotation, Core::Maths::Vec3D(1)) * Core::Maths::Vec4D(0, 0, 1, 0)).getVector().unitVector();
	lightManager->DLights.push_back(this);
	Light::Update(cameras, resources, textureManager, lightManager, deltaTime);
}

float* LowRenderer::DirectionalLight::GetValues()
{
	float* result = new float[DIRECTIONAL_SIZE];
	result[0] = AmbientLight.x;
	result[1] = AmbientLight.y;
	result[2] = AmbientLight.z;
	result[3] = DiffuseLight.x;
	result[4] = DiffuseLight.y;
	result[5] = DiffuseLight.z;
	result[6] = SpecularLight.x;
	result[7] = SpecularLight.y;
	result[8] = SpecularLight.z;
	result[9] = Smoothness;
	result[10] = GlobalDirection.x;
	result[11] = GlobalDirection.y;
	result[12] = GlobalDirection.z;
	return result;
}

void LowRenderer::PointLight::DeleteLight()
{
	Light::~Light();
	this->~PointLight();
}

void LowRenderer::PointLight::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime)
{
	Position = (Core::Maths::Vec4D(0, 0, 0, 1)).getVector();
	lightManager->PLights.push_back(this);
	Light::Update(cameras, resources, textureManager, lightManager, deltaTime);
}

float* LowRenderer::PointLight::GetValues()
{
	float* result = new float[POINT_SIZE];
	result[0] = Position.x;
	result[1] = Position.y;
	result[2] = Position.z;
	result[3] = AmbientLight.x;
	result[4] = AmbientLight.y;
	result[5] = AmbientLight.z;
	result[6] = DiffuseLight.x;
	result[7] = DiffuseLight.y;
	result[8] = DiffuseLight.z;
	result[9] = SpecularLight.x;
	result[10] = SpecularLight.y;
	result[11] = SpecularLight.z;
	result[12] = Smoothness;
	result[13] = Attenuation.x;
	result[14] = Attenuation.y;
	return result;
}

void LowRenderer::SpotLight::Update(std::vector<LowRenderer::Rendering::RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, Lightning::LightManager* lightManager, float deltaTime)
{
	Position = (Core::Maths::Vec4D(0, 0, 0, 1)).getVector();
	Core::Maths::Mat4D tranform = Core::Maths::Mat4D::CreateTransformMatrix(Core::Maths::Vec3D(), Rotation, Core::Maths::Vec3D(1));
	GlobalDirection = ((tranform * Core::Maths::Vec4D(0, 0, 1, 0))).getVector().unitVector();
	GlobalUp = ((tranform * Core::Maths::Vec4D(0, 1, 0, 0))).getVector().unitVector();
	lightManager->SLights.push_back(this);
	if (ShadowType == ShadowUpdateType::Baked || ShadowType == ShadowUpdateType::Realtime)
	{
		lightManager->ShadowMapLights.push_back(this);
	}
	Light::Update(cameras, resources, textureManager, lightManager, deltaTime);
}

float* LowRenderer::SpotLight::GetValues()
{
	float* result = new float[SPOT_SIZE];
	result[0] = Position.x;
	result[1] = Position.y;
	result[2] = Position.z;
	result[3] = AmbientLight.x;
	result[4] = AmbientLight.y;
	result[5] = AmbientLight.z;
	result[6] = DiffuseLight.x;
	result[7] = DiffuseLight.y;
	result[8] = DiffuseLight.z;
	result[9] = SpecularLight.x;
	result[10] = SpecularLight.y;
	result[11] = SpecularLight.z;
	result[12] = Smoothness;
	result[13] = Attenuation.x;
	result[14] = Attenuation.y;
	result[15] = GlobalDirection.x;
	result[16] = GlobalDirection.y;
	result[17] = GlobalDirection.z;
	result[18] = Spotangle;
	result[19] = Spotratio;
	return result;
}