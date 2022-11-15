#pragma once

#include <vector>

#include "ShaderProgram.hpp"

namespace Resources
{
	class ShaderManager
	{
	public:
		ShaderManager();
		~ShaderManager();

		size_t CreateShaderProgram(Resources::VertexShader* vertex, Resources::FragmentShader* fragment, const char* name);
		std::vector<ShaderProgram*> GetShaderPrograms();
		std::vector<ShaderProgram*> GetShaderPrograms(const char* filter);
		ShaderProgram* GetShaderProgram(const char* name);
		size_t GetShaderProgramIndex(const char* name);
		void AddShader(Resources::Shader* in);
		void ReloadShaders();
		void DestroyShaderPrograms();
	private:
		std::vector<ShaderProgram> shaderPrograms;
		std::vector<Resources::Shader*> shaders;
	};
}