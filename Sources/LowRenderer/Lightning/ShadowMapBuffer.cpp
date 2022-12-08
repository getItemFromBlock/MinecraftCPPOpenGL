#include "LowRenderer\Lightning\ShadowMapBuffer.hpp"

#include <glad/glad.h>
#include <ImGUI/imgui.h>

#include "Core/Debug/Log.hpp"
#include "Core/Debug/Assert.hpp"
#include "Resources/ResourceManager.hpp"
#include "Resources/TextureManager.hpp"

unsigned int LowRenderer::Lightning::ShadowMapBuffer::Resolution = 1024u;

LowRenderer::Lightning::ShadowMapBuffer::ShadowMapBuffer()
{
}

LowRenderer::Lightning::ShadowMapBuffer::~ShadowMapBuffer()
{
}

bool LowRenderer::Lightning::ShadowMapBuffer::Init(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &FBO);

	sizeX = width;
	sizeY = height;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		LOG("Error, could not create FrameBuffer : 0x%x\n", Status);
		return false;
	}
	loaded = true;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

void LowRenderer::Lightning::ShadowMapBuffer::BindForWriting()
{
	Assert(FBO != 0);
	glViewport(0,0,sizeX,sizeY);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void LowRenderer::Lightning::ShadowMapBuffer::BindForReading()
{
	glActiveTexture(GL_TEXTURE0 + textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void LowRenderer::Lightning::ShadowMapBuffer::Load(const char* path)
{
	fullPath = path;
	if (loaded) UnLoad();
	int startIndex = 0;
	char tmp;
	for (int i = 0; i < 255; i++)
	{
		tmp = path[i];
		if (tmp == 0)
		{
			break;
		}
		else if (tmp == '\\' || tmp == '/')
		{
			startIndex = i + 1;
		}
	}
	tmp = path[startIndex];
	int index = 0;
	for (int i = startIndex + 1; i < 255 && tmp != 0 && tmp != '.'; i++)
	{
		Name[index] = tmp;
		tmp = path[i];
		index++;
	}
	Name[index] = 0;
	Init(Resolution, Resolution);
	loaded = true;
}

void LowRenderer::Lightning::ShadowMapBuffer::UnLoad()
{
	if (!loaded) return;
	Resources::Texture::UnLoad();
	glDeleteFramebuffers(1, &FBO);
	this->~ShadowMapBuffer();
}

void LowRenderer::Lightning::ShadowMapBuffer::RefreshSize()
{
	if (sizeX != Resolution)
	{
		sizeX = Resolution;
		sizeY = Resolution;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeX, sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)0);
	}
}

void LowRenderer::Lightning::ShadowMapBuffer::Dump(const char* path)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	Core::Maths::Color4* buf = new Core::Maths::Color4[(size_t)(4llu * Resolution * Resolution)];
	unsigned char* bufTmp = new unsigned char[(size_t)(1llu * Resolution * Resolution)];
	glReadPixels(0, 0, Resolution, Resolution, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, bufTmp);
	for (size_t i = 0; i < 1llu * Resolution * Resolution; i++) buf[i] = Core::Maths::Color4(bufTmp[i], bufTmp[i], bufTmp[i]);
	Save(path, reinterpret_cast<unsigned char*>(buf), Resolution, Resolution);
	delete[] buf;
	delete[] bufTmp;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void LowRenderer::Lightning::ShadowMapBuffer::SetShadowMapResolution(unsigned int newRes)
{
	Resolution = newRes;
}