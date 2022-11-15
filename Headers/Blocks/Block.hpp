#pragma once

#include <string>

#include "Core/Maths/Maths.hpp"
#include "Core/Util/Sides.hpp"
#include "Core/Util/Shape.hpp"
#include "Core/Util/HitBox.hpp"

namespace World
{
	class World;
}

namespace Blocks
{
	class Block
	{
	public:
		Block(const char* Name_ID);
		~Block();

		virtual bool OnClick() { return false; }
		virtual void OnBreak() {}

		bool CanDestroy() { return breakable; }
		const char* GetName() { return name.c_str(); };
		virtual bool IsSideFull(Core::Util::Side side) { return true; }
		virtual bool IsLightBlock() { return false; }
		virtual Core::Maths::Vec3D GetLightValue() { return Core::Maths::Vec3D(); }
		virtual const Core::Util::Shape& GetFaceShape(Core::Util::Side side);
		virtual Core::Util::ShapeType GetShapeType();
		virtual const Core::Util::HitBox& GetHitBox();
		virtual Core::Util::CollisionType GetCollisionType();
		virtual void Update(World::World* worldIn, Blocks::Block* other, Core::Maths::Int3D otherPos);

		Block* SetHardness(float value);
		Block* SetUnbreakable();
		void RegisterModel(Resources::TextureAtlas* atlas);
	protected:
		Block();
		std::string name;
		float hardness = 1.0f;
		bool breakable = true;
		static bool IsValidString(const std::string& in);

		Core::Util::Shape shapes[7];
		Core::Util::HitBox hitBox;
	};
}