#include "SceneManager.h"
#include "CustomScript/CustomScriptInclude.h"
//���⿡ Ŀ���� ��ũ��Ʈ ��� include

void SceneManager::Custom_Test_Obj_Set()
{
	std::string cat_filepath = "Data\\Objects\\Test_cat\\12221_Cat_v1_l3.obj";
	std::string nanosuit_filepath = "Data\\Objects\\nanosuit\\nanosuit.obj";

	GameObject_v2 *gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 4, XMFLOAT3(10.0f, 10.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<cstest>();
	gameObjectBuffer.push_back(gameObj);

	gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 4, XMFLOAT3(40.0f, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	gameObj->AddComponent<cstest>();
	gameObjectBuffer.push_back(gameObj);

	for (int i = 0; i < 10; i++) {
		gameObj = new GameObject_v2(this, modelBuffer.buffer[1], 0, 0, XMFLOAT3(-20.0f * i, 0.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
		gameObj->AddComponent<cstest>();
		gameObj->AddComponent<cstest02>();
		gameObjectBuffer.push_back(gameObj);
	}
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
