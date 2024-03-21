#ifndef _LIGHT_FX
#define _LIGHT_FX

#include "00. Global.fx"

////////////
// Struct //
////////////
struct LightDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float3 direction;
    float padding;
};

struct MaterialDesc
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
};

/////////////////
// ConstBuffer //
/////////////////

cbuffer LightBuffer
{
    LightDesc GlobalLight;
};

cbuffer MaterialBuffer
{
    MaterialDesc Material;
};

/////////
// SRV //
/////////
Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

//////////////
// Function //
//////////////

float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{
    float4 ambientColor = 0;
    float4 diffuseColor = 0;
    float4 specularColor = 0;
    float4 emissiveColor = 0;
    
    // Ambient
    {
        float4 color = GlobalLight.ambient * Material.ambient;
        ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
    }
    
    // Diffuse
    {
        float4 color = DiffuseMap.Sample(LinearSampler, uv);
        float lambert = saturate(dot(-GlobalLight.direction, normal));
        diffuseColor = color * lambert * GlobalLight.diffuse * Material.diffuse;
    }
    
    // Specular
    {
        //float3 R = reflect(GlobalLight.direction, normal);
        float3 R = GlobalLight.direction - (normal * 2 * dot(normal, GlobalLight.direction));
        R = normalize(R);
        float4 color = GlobalLight.specular * Material.specular;
        float3 cameraPosition = CameraPosition();
        float3 E = normalize(cameraPosition - worldPosition);
        float specular = saturate(dot(E, R));
        specular = pow(specular, 10);
        specularColor = specular * color;
    }
    
    // Emissive
    {
        float4 color = GlobalLight.emissive * Material.emissive;
        float3 cameraPosition = CameraPosition();
        float3 E = normalize(cameraPosition - worldPosition);
        float value = saturate(dot(E, normal));
        float emissive = 1 - value;
        emissive = smoothstep(0.f, 1.f, emissive);
        emissive = pow(emissive, 2);
        emissiveColor = emissive * color;
    }
    
    return ambientColor + diffuseColor + specularColor + emissiveColor;
}
#endif