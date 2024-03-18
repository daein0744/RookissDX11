#pragma once
#include "Geometry.h"
#include "VertexData.h"
class GeometryHelper
{
public:
	static void CreateRetangle(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateRetangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color);
};

