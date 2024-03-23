#pragma once

struct ModelBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	shared_ptr<ModelBone> parent; // Cache

	Matrix transform;
	vector<shared_ptr<ModelBone>> children; // Cache
};

class ModelMesh
{
public:
	void CreateBuffers();

	wstring name;

	// Mesh
	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer = make_shared<VertexBuffer>();
	shared_ptr<IndexBuffer> indexBuffer = make_shared<IndexBuffer>();

	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ModelBone> bone; // Cache
};

