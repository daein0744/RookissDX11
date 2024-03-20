#pragma once
#include "ResourceBase.h"

class Shader;
class Texture;

class Material : public ResourceBase
{
    using Super = ResourceBase;
public:
    Material(ComPtr<ID3D11Device> device);
    virtual ~Material();

    shared_ptr<Shader> GetShader() { return _shader; }
    shared_ptr<Texture> GetTexture() { return _texture; }

    void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
    void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }

private:
    ComPtr<ID3D11Device> _device;

private:
    shared_ptr<Shader> _shader;
    shared_ptr<Texture> _texture;
};