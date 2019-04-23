#include "SceneManager.h"
#include "Engine.h"
#include "CustomScript/CustomScriptInclude.h"
#include "Graphics/Model.h"
#include "Graphics/Shaders.h"

using namespace DirectX;
//여기에 커스텀 스크립트 헤더 include

void SceneManager::Custom_Test_Obj_Set()
{
	std::string cat_filepath = "Data\\Objects\\Test_cat\\12221_Cat_v1_l3.obj";
	std::string nanosuit_filepath = "Data\\Objects\\nanosuit\\nanosuit.obj";

	GameObject_v2 *gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 4, XMFLOAT3(10.0f, 10.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<cstest>();
	gameObjectBuffer.push_back(gameObj);

	gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 4, XMFLOAT3(20.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<BoxCollider>();
	gameObj->GetComponent<BoxCollider>()->debugModel.Extents = DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f);
	//gameObj->AddComponent<cstest>();
	//gameObj->AddComponent<cstest02>();
	gameObjectBuffer.push_back(gameObj);

	gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 7, XMFLOAT3(20.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<BoxCollider>();
	gameObj->AddComponent<cstest>();
	gameObj->AddComponent<cstest02>();
	gameObjectBuffer.push_back(gameObj);

	gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 7, XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<Terrain>();
	gameObj->GetComponent<Terrain>()->heightFilePath = "Data\\Textures\\heightmap01.bmp";
	gameObjectBuffer.push_back(gameObj);

	for (int i = 0; i < 10; i++) {
		gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 0, XMFLOAT3(-20.0f * i, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		gameObj->AddComponent<cstest>();
		gameObj->AddComponent<cstest02>();
		gameObjectBuffer.push_back(gameObj);
	}

	//Light Obj
	gameObj = new GameObject_v2(this, modelBuffer.buffer[0], 0, 3, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<DirectionalLight>();
	gameObjectBuffer.push_back(gameObj);

	
}

void SceneManager::ClassifyComponent(Component * component)
{
	ScriptBehaviour *scriptCompo_test = dynamic_cast<ScriptBehaviour*>(component);
	if (scriptCompo_test != NULL) {
		engine->InsertScriptComponent(component);
		return;
	}

	Light_ver2 *lightCompo_test = dynamic_cast<Light_ver2*>(component);
	if (lightCompo_test != NULL) {
		engine->InsertLightComponent(lightCompo_test);
		return;
	}

	Terrain *terrain_test = dynamic_cast<Terrain*>(component);
	if (terrain_test != NULL) {
		engine->InsertTerrainComponent(terrain_test);
		return;
	}

	Collider *collider_test = dynamic_cast<Collider*>(component);
	if (collider_test != NULL) {
		engine->InsertColliderComponent(collider_test);
		return;
	}

	assert("this component is impossible to classfiy." && 1 == 0);
}

SceneManager::~SceneManager()
{
	for (std::vector<GameObject_v2*>::iterator it = gameObjectBuffer.begin(); it != gameObjectBuffer.end(); it++)
		delete *it;
}

ModelBuffer::~ModelBuffer()
{
	for (std::vector<Model*>::iterator it = buffer.begin(); it != buffer.end(); it++)
		delete *it;
}

TerrainModelBuffer::~TerrainModelBuffer()
{
	for (std::vector<Model*>::iterator it = buffer.begin(); it != buffer.end(); it++)
		delete *it;
}
