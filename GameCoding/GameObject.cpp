#include "pch.h"
#include "GameObject.h"
#include "Pipeline.h"
#include "MyMatrix.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "Transform.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device)
{
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry);

	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry->GetVertices());

	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry->GetIndices());

	_vertexShader = make_shared<VertexShader>(_device);
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	_inputLayout = make_shared<InputLayout>(_device);
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_device, deviceContext);
	_constantBuffer->Create();

	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	_pixelShader = make_shared<PixelShader>(_device);
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	_texture1 = make_shared<Texture>(_device);
	_texture1->Create(L"Skeleton.png");

	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();

	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();

	_transform = make_shared<Transform>();
	_parent = make_shared<Transform>();

	_transform->SetParent(_parent);
	_parent->AddChild(_transform);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	Vec3 pos = _parent->GetPosition();
	pos.x += 0.001f;
	_parent->SetPosition(pos);

	Vec3 rot = _parent->GetRotation();
	rot.z += 0.01f;
	_parent->SetRotation(rot);

	_transformData.matWorld = _transform->GetWorldMatrix();
	_constantBuffer->CopyData(_transformData);
}

void GameObject::Render(shared_ptr<class Pipeline> pipeline)
{
	PipelineInfo info;
	info.inputLayout = _inputLayout;
	info.vertexShader = _vertexShader;
	info.pixelShader = _pixelShader;
	info.rasterizerState = _rasterizerState;
	info.blendState = _blendState;
	pipeline->UpdatePipeline(info);
	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);
	pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
	pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}
