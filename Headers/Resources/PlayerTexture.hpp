#pragma once

#include "Texture.hpp"

namespace Resources
{
	class PlayerTexture : public Texture
	{
	public:
		PlayerTexture();
		~PlayerTexture();

		virtual void Load(const char* path) override;
		virtual void UnLoad() override;

	private:

	};
}