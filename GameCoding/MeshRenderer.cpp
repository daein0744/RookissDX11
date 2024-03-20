#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "InputLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "Pipeline.h"
#include "Game.h"
#include "Mesh.h"
#include "Material.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer)
	, _device(device)
	, _deviceContext(deviceContext)
{

}

MeshRenderer::~MeshRenderer()
{
	
}

void MeshRenderer::Render(shared_ptr<class Pipeline> pipeline)
{

}
