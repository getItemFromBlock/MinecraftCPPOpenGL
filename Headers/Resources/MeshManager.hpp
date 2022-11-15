#pragma once

#include <vector>
#include "Resources/ResourceManager.hpp"
#include "Core/DataStructure/INameable.hpp"

namespace Resources
{
	class Mesh;
}
namespace LowRenderer
{
	class Model;
}

namespace Resources
{
	class ModelHolder : public Core::DataStructure::INameable
	{
	public:
		ModelHolder();
		ModelHolder(LowRenderer::Model* in, const char* path);
		~ModelHolder();
		const char* GetName() override;
		LowRenderer::Model* model = nullptr;
	private:
		std::string fullPath = "";
	};

	class MeshManager
	{
	public:
		MeshManager();
		~MeshManager();
		size_t AddMesh(Mesh* in);
		std::vector<Mesh*> GetMeshs();
		std::vector<Mesh*> GetMeshs(const char* filter);
		size_t AddModel(LowRenderer::Model* in, const char* path);
		std::vector<ModelHolder*> GetModels();
		std::vector<ModelHolder*> GetModels(const char* filter);
		void ClearModels();
	private:
		std::vector<Mesh*> meshs;
		std::vector<ModelHolder*> models;
	};
}