#pragma once

#include "ResourceBase.h"

class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> device);
	~ResourceManager();

	void Init();

	template <typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template <typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template <typename T>
	shared_ptr<T> Get(const wstring& key);

	template <typename T>
	ResourceType GetResourceType();

private:
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();

private:
	ComPtr<ID3D11Device> _device;
	using KeyObjMap = map <wstring, shared_ptr<ResourceBase>>;


	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template<typename T>
inline shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	shared_ptr<T> obejct = make_shared<T>();
	obejct->Load(path);
	keyObjMap[key] = obejct;
	
	return obejct;
}

template<typename T>
inline bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];
	
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;
	return true;
}

template<typename T>
inline shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
inline ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, class Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, class Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, class Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, class Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, class Animation>)
		return ResourceType::Animation;

	assert(false);
	return ResourceType::None;
}
