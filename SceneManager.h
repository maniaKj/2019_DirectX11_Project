#pragma once
#include <vector>

class Engine; //include 정의 조심
class GameObject_v2;
struct TimeInfo;
class Component;
class Model;
class PixelShader;
class VertexShader;

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

class TerrainModelBuffer {
public:
	std::vector<Model*> buffer;
	~TerrainModelBuffer();
};

class SceneManager {
	friend class Engine;
public:
	//bool Initialize();
	//void LoadObject_Info();
	SceneManager(Engine * const engine_ptr, TimeInfo * const timeInfo) : engine(engine_ptr), Time(timeInfo) {}
	void Custom_Test_Obj_Set();
	void ClassifyComponent(Component * component);
	~SceneManager();
	
	TimeInfo * const Time;

private:
	std::vector<GameObject_v2*> gameObjectBuffer;
	Engine * const engine;
	ModelBuffer modelBuffer;
	ShaderBuffer shaderBuffer;
	TerrainModelBuffer terrainBuffer;
};