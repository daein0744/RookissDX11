#pragma once
#include "Component.h"
#include "Material.h"
#include "Shader.h"

class InputLayout;
class Texture;
class Mesh;
class Material;
class Shader;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer();

	void Render(shared_ptr<class Pipeline> pipeline);

	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetShader(shared_ptr<Shader> shader) { _material->SetShader(shader); }
	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetTexture(shared_ptr<Texture> texture) { _material->SetTexture(texture); }

	shared_ptr<Material> GetMaterial() { return _material; }
	shared_ptr<VertexShader> GetVertexShader() { return _material->GetShader()->GetVertexShader(); }
	shared_ptr<PixelShader> GetPixelShader() { return _material->GetShader()->GetPixelShader(); }
	shared_ptr<InputLayout> GetInputLayout() { return _material->GetShader()->GetInputLayout(); }

	shared_ptr<Mesh> GetMesh() { return _mesh; }
	shared_ptr<Texture> GetTexture() { return GetMaterial()->GetTexture(); }


private:
	friend class RenderManager;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	// Mesh
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
};