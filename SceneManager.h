#pragma once
#include <vector>
#include <memory>

class Engine; //include 정의 조심
class GameObject_v2;
struct TimeInfo;
class Component;
class Model;
class PixelShader;
class VertexShader;
class ScriptBehaviour;
class Light_ver2;
class Terrain;
class Collider;
class Animator;

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

	void ClassifyComponent(Component * _component, GameObject_v2 *_destination); //똑같은 컴포넌트를 여러 번 분류하는 경우 조심.
	void Component_Valid_Test();
	TimeInfo * const Time;

private:
	SceneManager(Engine * const engine_ptr, TimeInfo * const timeInfo) : engine(engine_ptr), Time(timeInfo) {}
	void Custom_Test_Obj_Set();
	~SceneManager();

	std::vector<GameObject_v2*> gameObjectBuffer;
	Engine * const engine;
	ModelBuffer modelBuffer;
	ShaderBuffer shaderBuffer;
	TerrainModelBuffer terrainBuffer;
};