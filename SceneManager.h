#pragma once
#include "Component/GameObject_v2.h"
#include "Graphics/Model.h"
#include "Graphics/Shaders.h"
#include "CustomScript/customScript01.h" //나중에 처리할 것
#include <vector>

class ShaderBuffer {
public:
	std::vector<PixelShader> pixelShaderBuffer;
	std::vector<VertexShader> vertexShaderBuffer;
};

class ModelBuffer {
public:
	std::vector<Model*> buffer;
	~ModelBuffer();
};

class SceneManager {
public:
	//bool Initialize();
	//void LoadObject_Info();
	void Custom_Test_Obj_Set();
	~SceneManager();


	std::vector<GameObject_v2*> gameObjectBuffer;
	ModelBuffer modelBuffer;
	ShaderBuffer shaderBuffer;
	
};