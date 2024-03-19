#pragma once

#include "ResourceBase.h"
#include "ShaderBase.h"

class Shader : public ResourceBase
{
	using Super = ResourceBase;
public:
	Shader();
	virtual ~Shader();
};

