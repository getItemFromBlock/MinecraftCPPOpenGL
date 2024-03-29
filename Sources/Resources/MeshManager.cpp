#include "Resources/MeshManager.hpp"

#include "Core/DataStructure/NameSearcher.hpp"
#include "Resources/Mesh.hpp"
#include "LowRenderer/Model.hpp"

Resources::MeshManager::MeshManager()
{
}

Resources::MeshManager::~MeshManager()
{
}

size_t Resources::MeshManager::AddMesh(Mesh* in)
{
	for (size_t i = 0; i < meshs.size(); i++)
	{
		if (meshs[i] == in)
		{
			return i;
		}
	}
	meshs.push_back(in);
	return meshs.size() - 1;
}

std::vector<Resources::Mesh*> Resources::MeshManager::GetMeshs()
{
	return meshs;
}

std::vector<Resources::Mesh*> Resources::MeshManager::GetMeshs(const char* filter)
{
	if (!filter || filter[0] == 0) return GetMeshs();
	return Core::DataStructure::NameSearcher::FindElementsPtr<Mesh>(meshs, filter);
}

size_t Resources::MeshManager::AddModel(LowRenderer::Model* in, const char* path)
{
	for (size_t i = 0; i < models.size(); i++)
	{
		if (models[i]->model == in)
		{
			return i;
		}
	}
	models.push_back(new ModelHolder(in, path));
	return models.size() - 1;
}

std::vector<Resources::ModelHolder*> Resources::MeshManager::GetModels()
{
	return models;
}

std::vector<Resources::ModelHolder*> Resources::MeshManager::GetModels(const char* filter)
{
	if (!filter || filter[0] == 0) return GetModels();
	return Core::DataStructure::NameSearcher::FindElementsPtr<ModelHolder>(models, filter);
}

Resources::ModelHolder::ModelHolder()
{
}

Resources::ModelHolder::~ModelHolder()
{
}

const char* Resources::ModelHolder::GetName()
{
	return fullPath.c_str();
}

Resources::ModelHolder::ModelHolder(LowRenderer::Model* in, const char* path)
{
	model = in;
	fullPath = path;
}

void Resources::MeshManager::ClearModels()
{
	for (size_t i = 0; i < models.size(); i++)
	{
		delete models[i]->model;
		delete models[i];
	}
	models.clear();
}