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

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device)
{
	_geometry = make_shared<Geometry<VertexColorData>>();
	GeometryHelper::CreateRectangle(_geometry, Color(0.f, 0.f, 1.f, 1.f));

	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry->GetVertices());

	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry->GetIndices());

	_vertexShader = make_shared<VertexShader>(_device);
	_vertexShader->Create(L"Color.hlsl", "VS", "vs_5_0");

	_inputLayout = make_shared<InputLayout>(_device);
	_inputLayout->Create(VertexColorData::descs, _vertexShader->GetBlob());

	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_device, deviceContext);
	_constantBuffer->Create();

	_rasterizerState = make_shared<RasterizerState>(_device);
	_rasterizerState->Create();

	_pixelShader = make_shared<PixelShader>(_device);
	_pixelShader->Create(L"Color.hlsl", "PS", "ps_5_0");

	_texture1 = make_shared<Texture>(_device);
	_texture1->Create(L"Skeleton.png");

	_samplerState = make_shared<SamplerState>(_device);
	_samplerState->Create();

	_blendState = make_shared<BlendState>(_device);
	_blendState->Create();
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	Matrix matScale = MyMatrix::CreateScale(_localScale / 3);
	Matrix matRotX = MyMatrix::CreateRotationX(XMConvertToRadians(_localRotation.x));
	Matrix matRotY = MyMatrix::CreateRotationY(XMConvertToRadians(_localRotation.y));
	Matrix matRotZ = MyMatrix::CreateRotationZ(XMConvertToRadians(_localRotation.z));
	Matrix matTranslation = MyMatrix::CreateTranslation(_localPosition);
	_transformData.matWorld = matTranslation * matRotX * matRotY * matRotZ * matScale;
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
