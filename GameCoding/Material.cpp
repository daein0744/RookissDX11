#include "pch.h"
#include "Material.h"
#include "InputLayout.h"
#include "Shader.h"
#include "Texture.h"

Material::Material(ComPtr<ID3D11Device> device) : Super(ResourceType::Material), _device(device)
{

}

Material::~Material()
{
}