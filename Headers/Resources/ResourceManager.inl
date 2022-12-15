#pragma once

#include <string>

#include "Core/Debug/Log.hpp"
//#include "ResourceManager.hpp"

void Resources::ResourceManager::Sanitize(std::string& item)
{
	while (true)
	{
		size_t start_pos = item.find("\\\\");
		if (start_pos == std::string::npos)
		{
			start_pos = item.find("\\");
			if (start_pos == std::string::npos)
			{
				break;
			}
			else
				item.replace(start_pos, 1, "/");
		}
		else
			item.replace(start_pos, 2, "/");
	}
	for (int i = 0; i < item.size(); i++)
	{
		item[i] = tolower(item[i]);
	}
}

template<typename T>
T* Resources::ResourceManager::Create(const char* path, bool load)
{
	T* out = new T();
	IResource* res = dynamic_cast<IResource*>(out);
	Assert(res != nullptr);
	delete out;
	std::string fullPath;
	if (!raw)
	{
		fullPath = "Resources/";
		fullPath.append(path);
	}
	else
	{
		fullPath = path;
	}
	Sanitize(fullPath);
	auto result = dataMap.find(fullPath.c_str());
	if (result != dataMap.end() && result->second)
	{
		result->second.get()->UnLoad();
		result->second.reset();
	}
	std::unique_ptr<IResource> ptr = std::make_unique<T>();
	if (load)
	{
		ptr->Load(fullPath.c_str());
		ptr->EndLoad();
		ptr->loaded.Store(true);
	}
	else ptr->loaded.Store(false);
	dataMap.emplace(fullPath, std::move(ptr));
	return dynamic_cast<T*>(dataMap.find(fullPath.c_str())->second.get());
}

template<typename T>
T* Resources::ResourceManager::Get(const char* path)
{
	std::string fullPath = path;
	Sanitize(fullPath);
	auto result = dataMap.find(fullPath.c_str());
	if (result == dataMap.end() || !result->second)
		return nullptr;
	IResource* ptr = result->second.get();
	if (!ptr->IsLoaded() && ptr->priority >= 0)
	{
		ptr->priority += 1;
	}
	T* res = dynamic_cast<T*>(result->second.get());
	Assert(res != nullptr);
	return res;
}

inline void Resources::ResourceManager::Delete(const char* path)
{
	std::string fullPath = path;
	Sanitize(fullPath);
	auto result = dataMap.find(fullPath.c_str());
	if (result == dataMap.end() || !result->second) return;
	result->second.get()->UnLoad();
	result->second.reset();
	dataMap.erase(fullPath.c_str());
}

inline void Resources::ResourceManager::SetPathAutoAppend(bool value)
{
	raw = value;
}

inline void Resources::ResourceManager::ClearResources()
{
	std::unordered_map<std::string, std::unique_ptr<IResource>>::iterator it = dataMap.begin();
	while (it != dataMap.end())
	{
		if (!it->second || !it->second.get()) return;
		it->second.get()->UnLoad();
		it->second.reset();
		it++;
	}
	dataMap.clear();
}

inline void Resources::ResourceManager::PushResourceToLoader(IResource* resIn, const char* path)
{
	loadingQueue.push_front(Resources::ResourceMHolder{ resIn, std::string(path) });
	res_count++;
}

inline void Resources::ResourceManager::Update()
{
	for (char i = 0; i < 8; i++)
	{
		Core::Util::ThreadHandler& th = pool[i];
		if (th.locked.Load() == true)
		{
			if (th.res)
			{
				th.res->EndLoad();
				LOG("Loaded %s", th.res->GetPath());
			}
			th.res = nullptr;
			if (loadingQueue.empty())
			{
				continue;
			}
			int max = -1;
			Resources::ResourceMHolder holder;
			for (auto t = loadingQueue.begin(); t != loadingQueue.end(); t++)
			{
				if (t->res->priority > max)
				{
					max = t->res->priority;
					holder = *t;
				}
			}
			Assert(holder.path.c_str()[0]);
			th.SetResource(holder.res, holder.path.c_str());
			for (auto t = loadingQueue.before_begin(); t != loadingQueue.end(); t++)
			{
				auto n = t;
				n++;
				if ((*n).path == holder.path)
				{
					loadingQueue.erase_after(t);
					break;
				}
			}
			holder.res->priority = -1;
			th.locked.Store(false);
		}
	}
}

inline void Resources::ResourceManager::Terminate()
{
	for (char i = 0; i < 8; i++)
	{
		IResource* res = pool[i].res;
		pool[i].shouldExit.Store(true);
		pool[i].locked.Store(false);
		pool[i].~ThreadHandler();
		if (res)
		{
			res->EndLoad();
			res->UnLoad();
		}
	}
	if (!loadingQueue.empty())
	{
		for (auto i = loadingQueue.begin(); i != loadingQueue.end(); i++)
		{
			if (dataMap.find(i->path.c_str()) == dataMap.end())
			{
				i->res->UnLoad();
				i->res->DeleteElement();
			}
		}
		loadingQueue.clear();
	}
}

inline float Resources::ResourceManager::GetLoadingCompletion()
{
	size_t total = 0;
	if (!loadingQueue.empty())
	{
		for (auto i = loadingQueue.begin(); i != loadingQueue.end(); i++)
		{
			total++;
		}
	}
	for (char i = 0; i < 8; i++)
	{
		if (!pool[i].locked.Load()) total++;
	}
	return 1.0f - total * 1.0f / res_count;
}
