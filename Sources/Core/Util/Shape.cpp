#include "Core/Util/Shape.hpp"

void Core::Util::Shape::AddToChunk(Core::Maths::Int3D blockPos, std::vector<Core::Util::Vertice>* vertices) const
{
	for (size_t i = 0; i < ShapeData.size(); i++)
	{
		vertices->push_back(ShapeData.at(i));
		vertices->back().pos = vertices->back().pos + Core::Maths::Vec3D(blockPos);
	}
}

void Core::Util::Shape::CreateCubeShape(Shape in[6])
{
    static const Core::Maths::Vec3D p[] =
    {
        Core::Maths::Vec3D(0, 0, 0), Core::Maths::Vec3D(1, 0, 0),
        Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec3D(1, 1, 0),
        Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec3D(1, 0, 1),
        Core::Maths::Vec3D(0, 1, 1), Core::Maths::Vec3D(1, 1, 1),
    };
    static const Core::Maths::Vec3D n[] =
    {
        Core::Maths::Vec3D(1,0,0), Core::Maths::Vec3D(-1,0,0),
        Core::Maths::Vec3D(0,1,0), Core::Maths::Vec3D(0,-1,0),
        Core::Maths::Vec3D(0,0,1), Core::Maths::Vec3D(0,0,-1),
    };
    static const Core::Maths::Vec2D u[] =
    {
        Core::Maths::Vec2D(0,0),
        Core::Maths::Vec2D(1,0),
        Core::Maths::Vec2D(0,1),
        Core::Maths::Vec2D(1,1),
    };
    in[0].ShapeData.push_back(Core::Util::Vertice(p[3], n[0], u[2]));
    in[0].ShapeData.push_back(Core::Util::Vertice(p[7], n[0], u[3]));
    in[0].ShapeData.push_back(Core::Util::Vertice(p[5], n[0], u[1]));
    in[0].ShapeData.push_back(Core::Util::Vertice(p[3], n[0], u[2]));
    in[0].ShapeData.push_back(Core::Util::Vertice(p[5], n[0], u[1]));
    in[0].ShapeData.push_back(Core::Util::Vertice(p[1], n[0], u[0]));

    in[1].ShapeData.push_back(Core::Util::Vertice(p[2], n[1], u[0]));
    in[1].ShapeData.push_back(Core::Util::Vertice(p[7], n[1], u[3]));
    in[1].ShapeData.push_back(Core::Util::Vertice(p[3], n[1], u[2]));
    in[1].ShapeData.push_back(Core::Util::Vertice(p[2], n[1], u[0]));
    in[1].ShapeData.push_back(Core::Util::Vertice(p[6], n[1], u[1]));
    in[1].ShapeData.push_back(Core::Util::Vertice(p[7], n[1], u[3]));

    in[2].ShapeData.push_back(Core::Util::Vertice(p[7], n[2], u[2]));
    in[2].ShapeData.push_back(Core::Util::Vertice(p[6], n[2], u[3]));
    in[2].ShapeData.push_back(Core::Util::Vertice(p[4], n[2], u[1]));
    in[2].ShapeData.push_back(Core::Util::Vertice(p[7], n[2], u[2]));
    in[2].ShapeData.push_back(Core::Util::Vertice(p[4], n[2], u[1]));
    in[2].ShapeData.push_back(Core::Util::Vertice(p[5], n[2], u[0]));
    
    in[3].ShapeData.push_back(Core::Util::Vertice(p[2], n[3], u[2]));
    in[3].ShapeData.push_back(Core::Util::Vertice(p[4], n[3], u[1]));
    in[3].ShapeData.push_back(Core::Util::Vertice(p[6], n[3], u[3]));
    in[3].ShapeData.push_back(Core::Util::Vertice(p[2], n[3], u[2]));
    in[3].ShapeData.push_back(Core::Util::Vertice(p[0], n[3], u[0]));
    in[3].ShapeData.push_back(Core::Util::Vertice(p[4], n[3], u[1]));

    in[4].ShapeData.push_back(Core::Util::Vertice(p[0], n[4], u[0]));
    in[4].ShapeData.push_back(Core::Util::Vertice(p[1], n[4], u[2]));
    in[4].ShapeData.push_back(Core::Util::Vertice(p[5], n[4], u[3]));
    in[4].ShapeData.push_back(Core::Util::Vertice(p[0], n[4], u[0]));
    in[4].ShapeData.push_back(Core::Util::Vertice(p[5], n[4], u[3]));
    in[4].ShapeData.push_back(Core::Util::Vertice(p[4], n[4], u[1]));

    in[5].ShapeData.push_back(Core::Util::Vertice(p[3], n[5], u[2]));
    in[5].ShapeData.push_back(Core::Util::Vertice(p[0], n[5], u[1]));
    in[5].ShapeData.push_back(Core::Util::Vertice(p[2], n[5], u[3]));
    in[5].ShapeData.push_back(Core::Util::Vertice(p[3], n[5], u[2]));
    in[5].ShapeData.push_back(Core::Util::Vertice(p[1], n[5], u[0]));
    in[5].ShapeData.push_back(Core::Util::Vertice(p[0], n[5], u[1]));
}

void Core::Util::Shape::CreateFaceShape(char side, Shape& in, Core::Maths::Vec3D fr, Core::Maths::Vec3D to, Core::Maths::Vec2D a, Core::Maths::Vec2D b)
{
    switch (side)
    {
    case 0:
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, fr.z), Core::Maths::Vec3D(1, 0, 0), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, to.z), Core::Maths::Vec3D(1, 0, 0), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, to.z), Core::Maths::Vec3D(1, 0, 0), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, fr.z), Core::Maths::Vec3D(1, 0, 0), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, to.z), Core::Maths::Vec3D(1, 0, 0), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, fr.z), Core::Maths::Vec3D(1, 0, 0), Core::Maths::Vec2D(a.x, a.y)));
        break;
    case 1:
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, fr.z), Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec2D(a.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, to.z), Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, fr.z), Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, fr.z), Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec2D(a.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, to.z), Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, to.z), Core::Maths::Vec3D(0, 1, 0), Core::Maths::Vec2D(b.x, b.y)));
        break;
    case 2:
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, to.z), Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, to.z), Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, to.z), Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, to.z), Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, to.z), Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, to.z), Core::Maths::Vec3D(0, 0, 1), Core::Maths::Vec2D(a.x, a.y)));
        break;
    case 3:
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, fr.z), Core::Maths::Vec3D(-1, 0, 0), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, to.z), Core::Maths::Vec3D(-1, 0, 0), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, to.z), Core::Maths::Vec3D(-1, 0, 0), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, fr.z), Core::Maths::Vec3D(-1, 0, 0), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, fr.z), Core::Maths::Vec3D(-1, 0, 0), Core::Maths::Vec2D(a.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, to.z), Core::Maths::Vec3D(-1, 0, 0), Core::Maths::Vec2D(b.x, a.y)));
        break;
    case 4:
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, fr.z), Core::Maths::Vec3D(0, -1, 0), Core::Maths::Vec2D(a.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, fr.z), Core::Maths::Vec3D(0, -1, 0), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, to.z), Core::Maths::Vec3D(0, -1, 0), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, fr.z), Core::Maths::Vec3D(0, -1, 0), Core::Maths::Vec2D(a.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, to.z), Core::Maths::Vec3D(0, -1, 0), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, to.z), Core::Maths::Vec3D(0, -1, 0), Core::Maths::Vec2D(b.x, a.y)));
        break;
    case 5:
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, fr.z), Core::Maths::Vec3D(0, 0, -1), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, fr.z), Core::Maths::Vec3D(0, 0, -1), Core::Maths::Vec2D(b.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, to.y, fr.z), Core::Maths::Vec3D(0, 0, -1), Core::Maths::Vec2D(b.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, to.y, fr.z), Core::Maths::Vec3D(0, 0, -1), Core::Maths::Vec2D(a.x, b.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(to.x, fr.y, fr.z), Core::Maths::Vec3D(0, 0, -1), Core::Maths::Vec2D(a.x, a.y)));
        in.ShapeData.push_back(Core::Util::Vertice(Core::Maths::Vec3D(fr.x, fr.y, fr.z), Core::Maths::Vec3D(0, 0, -1), Core::Maths::Vec2D(b.x, a.y)));
        break;
    default:
        break;
    }
}
