#include "pch.h"
#include "Mesh.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Mesh::Mesh(ComPtr<ID3D11Device> device)
	: Super(ResourceType::Mesh), _device(device)
{

}

Mesh::~Mesh()
{

}

void Mesh::CreateDefaultRectangle()
{
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry);

	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry->GetIndices());
}
