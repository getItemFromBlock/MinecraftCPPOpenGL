#pragma once

#include <string>

#include "Core/Util/Signal.hpp"

namespace Resources
{
	class ResourceManager;
	class IResource
	{
	public:
		virtual void Load(const char* path) = 0;
		virtual void EndLoad() {};
		virtual void DeleteElement() {};
		virtual void UnLoad() = 0;
		virtual const char* GetPath() = 0;
		bool IsLoaded() { return loaded.Load(); }
		void IncreasePriority(int value) { priority += value; }
	protected:
		int priority = 0;
		Core::Util::Signal loaded = false;
		friend class Resources::ResourceManager;
	};

	struct ResourceMHolder
	{
		Resources::IResource* res = nullptr;
		std::string path;
	};
}
