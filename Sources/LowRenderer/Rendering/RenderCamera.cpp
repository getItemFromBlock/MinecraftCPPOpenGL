#include "LowRenderer/Rendering/RenderCamera.hpp"

#include <ImGUI/imgui.h>

#include "Resources/TextureManager.hpp"
#include "Core/Util/TextHelper.hpp"

using namespace Core::Maths;

LowRenderer::Rendering::RenderCamera::RenderCamera()
{
	Resolution = Int2D(256, 256);
}

LowRenderer::Rendering::RenderCamera::~RenderCamera()
{
}

void LowRenderer::Rendering::RenderCamera::Update(std::vector<RenderCamera*>* cameras, Resources::ResourceManager* resources, Resources::TextureManager* textureManager, LowRenderer::Lightning::LightManager* lightManager, float deltaTime)
{
	if (!UUID[0])
	{
		for (unsigned int i = 0; i < 16; i++)
		{
			int c = (int)(rand() * 36.0f / RAND_MAX);
			UUID[i] = c < 10 ? c + '0' : c + 'A' - 10;
		}
		UUID[16] = 0;
	}
	if (!buffer)
	{
		std::string path = "RenderCamera";
		path.append("@");
		path.append(UUID);
		buffer = resources->Get<FrameBuffer>(path.c_str());
		if (buffer)
		{
			buffer->Update(Resolution.x, Resolution.y, Filter, Wrap);
		}
		else
		{
			buffer = resources->Create<FrameBuffer>(path.c_str());
			textureManager->AddFrameBuffer(buffer);
			buffer->Init(Resolution.x, Resolution.y, Filter, Wrap);
		}
	}
	Resolution = Int2D(Util::maxI(Resolution.x,1), Util::maxI(Resolution.y, 1));
	/*
	* TODO
	position = container->GetGlobalMatrix()->GetPositionFromTranslation();
	Vec3D cameraRot = container->GetGlobalMatrix()->GetRotationFromTranslation(container->GetGlobalMatrix()->GetScaleFromTranslation());
	rotation = Vec3D(cameraRot.y - 180, cameraRot.x, cameraRot.z);
	Core::App::Inputs i;
	i.ScreenSize = Resolution;
	Camera::Update(i,deltaTime);
	cameras->push_back(this);
	*/
}

void LowRenderer::Rendering::RenderCamera::DeleteCamera()
{
	Camera::~Camera();
	this->~RenderCamera();
}