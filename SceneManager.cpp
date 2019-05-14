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

	GameObject_v2 *gameObj = new GameObject_v2(this, modelBuffer.buffer[8], 0, 0, XMFLOAT3(0.0f, -30.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	//gameObj->AddComponent<cstest>();
	gameObj->AddComponent<Animator>();
	gameObj->GetComponent<Animator>()->SetAnimClip(&engine->GetAnimClipBuffer()->at(0));
	gameObj->transform.scale = DirectX::XMFLOAT3(0.3f, 0.3f, 0.3f);
	//gameObj->AddComponent<MemoryBOOM>();
	//gameObj->AddComponent<BoxCollider>();
	gameObjectBuffer.push_back(gameObj);

	for (int i = 0; i < 4; i++) {
		gameObj = new GameObject_v2(this, modelBuffer.buffer[6], 1, 0, XMFLOAT3(-20.0f * i, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		gameObj->AddComponent<cstest>();
		gameObj->AddComponent<cstest02>();
		//gameObj->AddComponent<BoxCollider>();
		//gameObj->RemoveComponent<BoxCollider>();
		gameObjectBuffer.push_back(gameObj);
	}

	//Simple Geometry Array
	for (int i = 0; i < 5; i++) {
		gameObj = new GameObject_v2(this, modelBuffer.buffer[i], 1, 6, XMFLOAT3(20.0f + i * 5.0f, 5.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		gameObj->AddComponent<SphereCollider_ver2>();
		gameObjectBuffer.push_back(gameObj);
	}

	//Character Controller
	gameObj = new GameObject_v2(this, modelBuffer.buffer[2], 1, 6, XMFLOAT3(19.0f, 20.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<CharacterController>();
	gameObj->AddComponent<GroundTest>();
	gameObjectBuffer.push_back(gameObj);

	//RigidBody
	gameObj = new GameObject_v2(this, modelBuffer.buffer[0], 1, 6, XMFLOAT3(20.3f, 15.0f, 10.0f), XMFLOAT3(45.0f, 0.0f, 0.0f));
	gameObj->AddComponent<BoxRigidBody>();
	gameObjectBuffer.push_back(gameObj);

	//RigidBody
	gameObj = new GameObject_v2(this, modelBuffer.buffer[0], 1, 6, XMFLOAT3(20.3f, 19.0f, 10.0f), XMFLOAT3(45.0f, 0.0f, 0.0f));
	gameObj->AddComponent<SphereRigidBody>();
	gameObjectBuffer.push_back(gameObj);

	//floor
	gameObj = new GameObject_v2(this, modelBuffer.buffer[0], 1, 6, XMFLOAT3(20.0f, -.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->transform.SetScale(15.0f, 1.0f, 15.0f);
	gameObj->AddComponent<BoxCollider_ver2>();
	gameObjectBuffer.push_back(gameObj);

	gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 1, 7, XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<Terrain>();
	gameObj->GetComponent<Terrain>()->heightFilePath = "Data\\Textures\\heightmap01.bmp";
	gameObjectBuffer.push_back(gameObj);

	//Light Obj
	gameObj = new GameObject_v2(this, modelBuffer.buffer[5], 1, 3, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<DirectionalLight>();
	gameObjectBuffer.push_back(gameObj);

	
}

void SceneManager::ClassifyComponent(Component * _component, GameObject_v2 * _destination)
{
	ScriptBehaviour *scriptCompo_test = dynamic_cast<ScriptBehaviour*>(_component);
	if (scriptCompo_test != NULL) {
		engine->InsertScriptComponent(scriptCompo_test, _destination);
		return;
	}

	Light_ver2 *lightCompo_test = dynamic_cast<Light_ver2*>(_component);
	if (lightCompo_test != NULL) {
		engine->InsertLightComponent(lightCompo_test, _destination);
		return;
	}

	Terrain *terrain_test = dynamic_cast<Terrain*>(_component);
	if (terrain_test != NULL) {
		engine->InsertTerrainComponent(terrain_test, _destination);
		return;
	}

	Collider *collider_test = dynamic_cast<Collider*>(_component);
	if (collider_test != NULL) {
		engine->InsertColliderComponent(collider_test, _destination);
		return;
	}

	Collider_v2 *colliderv2_test = dynamic_cast<Collider_v2*>(_component);
	if (colliderv2_test != NULL) {
		engine->InsertCollider_v2Component(colliderv2_test, _destination);
		return;
	}

	Animator *animator_test = dynamic_cast<Animator*>(_component);
	if (animator_test != NULL) {
		engine->InsertAnimatorComponent(animator_test, _destination);
		return;
	}

	assert("this component is impossible to classfiy." && 1 == 0);

}

void SceneManager::Component_Valid_Test()
{
	engine->Component_Valid_Test();
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
