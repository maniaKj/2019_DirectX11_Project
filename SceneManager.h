#pragma once
#include "Graphics/Model.h"
#include "Graphics/Shaders.h"
#include <vector>

class Engine; //include 정의 조심
class GameObject_v2;
struct TimeInfo;

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
	SceneManager(Engine * const engine_ptr, TimeInfo * const timeInfo) : engine(engine_ptr), Time(timeInfo) {}
	void Custom_Test_Obj_Set();
	~SceneManager();


	std::vector<GameObject_v2*> gameObjectBuffer;
	ModelBuffer modelBuffer;
	ShaderBuffer shaderBuffer;
	Engine * const engine;
	TimeInfo * const Time;
	
};