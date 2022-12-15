#pragma once

#include <string>

#include "Core/DataStructure/INameable.hpp"
#include "Core/Maths/Maths.hpp"
#include "Resources/Texture.hpp"
#include "Resources/ResourceManager.hpp"
#include "Resources/TextureManager.hpp"

namespace LowRenderer
{
	class Model;
	class SkinnedModel;
}

namespace Resources
{
	class Material : public Core::DataStructure::INameable
	{
	public:
		Core::Maths::Vec3 AmbientColor = Core::Maths::Vec3(1);	// Ka parameter in OBJ
		Core::Maths::Vec3 DiffuseColor = Core::Maths::Vec3(1);	// Kd parameter
		Core::Maths::Vec3 SpecularColor = Core::Maths::Vec3(1); // Ks parameter
		Core::Maths::Vec3 EmissiveColor = Core::Maths::Vec3(0);	// Ke parameter
		float Alpha = 1.0f;				// d parameter (or 1 - Tr)
		float Smoothness = 64.0f;		// Ns parameter
		float Shininess = 1.0f;
		float Absorbtion = 1.0f;

		Material();
		Material(const Material& other);
		~Material();
		void SetPath(const char* path);
		void Update(ResourceManager* manager);
		const std::string& GetPath() const;
		const char* GetName() override;
		void SetTexture(const char* path);
		void SetTexture(Texture* tex);
		void SetNormalMap(const char* path);
		void SetNormalMap(Texture* tex);
		Texture* GetTexture() const;
		Texture* GetNormalMap() const;
		void RenderGUI(TextureManager* textureManager);
		void Deserialize(ResourceManager* resources, const char* data, int64_t& pos, int64_t size, int64_t& line, std::string& err);
		void Serialize(std::ofstream& fileOut, unsigned int rec);
		void TexturePopUp(TextureManager* textureManager, bool normal);
		void SetSearchData(Texture* searchIndex);
		static void SetDebugTexture(Texture* tex, Texture* nrm);

		Material& operator=(const Material& other);
	private:
		char deltaF = 0;
		std::string name;
		std::string fullPath;
		std::string tmpTexturePath = "";
		std::string tmpNTexturePath = "";
		Texture* texture = nullptr; // map_Kd or map_Ka parameter (if present in file)
		Texture* normalMap = nullptr; // map_Kn (if present in file AND # GenerateTangent is present in file header AND tangent shader is used)
		static char searchText[64];
		static Texture* selectedTexture;
		static Texture* debugTexture;
		static Texture* debugNormal;

		friend class LowRenderer::Model;
		friend class LowRenderer::SkinnedModel;
	};
}
