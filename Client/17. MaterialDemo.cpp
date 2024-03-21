#include "pch.h"
#include "17. MaterialDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "RenderManager.h"
#include "Material.h"
void MaterialDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"13. Lighting.fx");
	// Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		{
			material->SetShader(_shader);
			auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
			material->SetDiffuseMap(texture);
		}

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(1.f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		desc.diffuse = Vec4(1.f);

		RESOURCES->Add<Material>(L"Veigar", material);
	}

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3(0.f, 0.f, -10.f));
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	// Object
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform();
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
		auto material = RESOURCES->Get<Material>(L"Veigar")->Clone();
		_obj->GetMeshRenderer()->SetMaterial(material);
	}

	// Object2
	_obj2 = make_shared<GameObject>();
	_obj2->GetOrAddTransform()->SetPosition(Vec3(0.5f, 0.5f, 2.f));
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
		auto material = RESOURCES->Get<Material>(L"Veigar")->Clone();
		_obj2->GetMeshRenderer()->SetMaterial(material);
	}
	RENDER->Init(_shader);
}

void MaterialDemo::Update()
{
	_camera->Update();

	{
		LightDesc desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f, 1.f, 1.f, 1.f);
		desc.direction = Vec3(0.f, -1.f, 0.f);
		desc.emissive = Vec4(1.f);
		RENDER->PushLightData(desc);
	}

	_obj->Update();
	_obj2->Update();
	RENDER->Update();
}

void MaterialDemo::Render()
{

}