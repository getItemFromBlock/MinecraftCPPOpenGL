#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <forward_list>

#include "IResource.hpp"
#include "Core/Debug/Assert.hpp"
#include "Core/Util/ThreadHandler.hpp"

namespace Resources
{
	class ResourceManager
	{
	public:

		template <typename T> T* Create(const char* path, bool load = true);
		template <typename T> T* Get(const char* path);
		inline void Delete(const char* path);

		inline void SetPathAutoAppend(bool value);
		inline void ClearResources();
		inline void PushResourceToLoader(IResource* resIn, const char* path);
		inline void Update();
		inline void Terminate();
		inline static void Sanitize(std::string& item);
		inline float GetLoadingCompletion();
	private:
		std::unordered_map<std::string, std::unique_ptr<IResource>> dataMap;
		bool raw = false;
		size_t res_count = 0;
		std::forward_list<Resources::ResourceMHolder> loadingQueue;
		Core::Util::ThreadHandler pool[8];
	};
}

#include "ResourceManager.inl"
