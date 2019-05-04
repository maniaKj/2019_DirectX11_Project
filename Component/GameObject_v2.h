#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object.h"
#include "Component.h"
#include "../SceneManager.h"
#include <vector>

struct GAMEOBJECT_INIT_DESC {
	std::string *obj_name;
	std::string *obj_id;
	SceneManager * const scene_manager;
	Model * model;
	const int & vshaderID = 0;
	const int & pshaderID = 0;
	const DirectX::XMFLOAT3 & pos;
	const DirectX::XMFLOAT3 & rot;
};

class GameObject_v2 : public Object {
	friend class Engine;
public:
	GameObject_v2(SceneManager * const sceneM,  Model * model, const int & vshaderID, const int & pshaderID, const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3 & rot);
	GameObject_v2(GAMEOBJECT_INIT_DESC & desc);
	~GameObject_v2();

	template<class T>
	T* AddComponent();
	template<class T>
	T* GetComponent();
	//void CustomSendMessage();

	std::string GameObjectName;
	std::string GameObjectID;

	bool enabled = true;
	Renderer renderer;
	Transform transform;

private:
	std::vector<Component*> componentBuffer;
	SceneManager * const sceneManager;
};

template<class T>
inline T* GameObject_v2::AddComponent()
{
	{//�̹� �ش� ������Ʈ�� ������ �ִ��� Ȯ��
		for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
			T *tmp = dynamic_cast<T*>(*it);
			if (tmp != NULL) {
				assert("Add Component : This component is already exists!");
			}
		}
	}

	T *tmp_class = new T;
	const COMPONENT_INIT_DESC compo_desc = {NULL, this, sceneManager->Time, &transform, &renderer };
	tmp_class->initialize(compo_desc); //���߿� ��ũ�� ������ Ȱ��
	Component * component = dynamic_cast<Component*>(tmp_class); // T�� Component�� �� ����ó��, ������Ʈ�� ����ȯ ������ ����

	std::string s = "asdf"; //���߿� ������Ʈ �̸� ��, ������Ʈ �̸� �߰��ؼ� ����
	assert("Add Component have Failed." && component != NULL);
	
	componentBuffer.push_back(component);
	sceneManager->ClassifyComponent(component);
	return tmp_class;
}

template<class T>
inline T* GameObject_v2::GetComponent()
{
	for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
		T *tmp = dynamic_cast<T*>(*it);
		if (tmp != NULL) {
			return tmp;
		}
	}

	assert("GetComponent have Failed. : This Component Doesn't Exists!" && 1 == 0); //���⼭ ���� �����ϱ� �Ʒ� �ڵ�� ����x
	return nullptr;
}

#endif

