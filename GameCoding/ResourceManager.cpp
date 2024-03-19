#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "ShaderBase.h"

ResourceManager::ResourceManager(ComPtr<ID3D11Device> device) : _device(device)
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultAnimation();
}

void ResourceManager::CreateDefaultTexture()
{
	{
		auto texture = make_shared<Texture>(_device);
		texture->SetName(L"Skeleton");
		texture->Create(L"Skleton.png");
		Add(texture->GetName(), texture);
	}
	
}

void ResourceManager::CreateDefaultMesh()
{
}

void ResourceManager::CreateDefaultShader()
{
}

void ResourceManager::CreateDefaultMaterial()
{
}

void ResourceManager::CreateDefaultAnimation()
{
}
