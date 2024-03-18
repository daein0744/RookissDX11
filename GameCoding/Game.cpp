#include "pch.h"
#include "Game.h"
#include "MyMatrix.h"
#include "Graphics.h"
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
#include "Pipeline.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd;

	_graphics = make_shared<Graphics>(hwnd);
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());
	_geometry = make_shared<Geometry<VertexTextureData>>();
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_texture1 = make_shared<Texture>(_graphics->GetDevice());
	_rasterizerState = make_shared<RasterizerState>(_graphics->GetDevice());
	_samplerState = make_shared<SamplerState>(_graphics->GetDevice());
	_blendState = make_shared<BlendState>(_graphics->GetDevice());
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

	GeometryHelper::CreateRectangle(_geometry);
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer->Create(_geometry->GetIndices());
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");
	_texture1->Create(L"Skeleton.png");
	_constantBuffer->Create();
	_rasterizerState->Create();
	_samplerState->Create();
	_blendState->Create();
}

void Game::Update()
{
	 Matrix matScale = MyMatrix::CreateScale(_localScale);
	 Matrix matRotX = MyMatrix::CreateRotationX(XMConvertToRadians(_localRotation.x));
	 Matrix matRotY = MyMatrix::CreateRotationY(XMConvertToRadians(_localRotation.y));
	 Matrix matRotZ = MyMatrix::CreateRotationZ(XMConvertToRadians(_localRotation.z));
	 Matrix matTranslation = MyMatrix::CreateTranslation(_localPosition);
	 _transformData.matWorld = matTranslation * matRotX * matRotY * matRotZ * matScale;
	 _constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphics->RenderBegin();
	{
		auto _deviceContext = _graphics->GetDeviceContext();
		// RS, OM
		_graphics->RenderBegin();

		PipelineInfo info;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.pixelShader = _pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;
		_pipeline->UpdatePipeline(info);
		_pipeline->SetVertexBuffer(_vertexBuffer);
		_pipeline->SetIndexBuffer(_indexBuffer);
		_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
		_pipeline->SetTexture(0, SS_PixelShader, _texture1);
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
		_pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}
	_graphics->RenderEnd();
}