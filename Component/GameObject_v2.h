#pragma once

#include "Object.h"
#include "Component.h"
#include "../SceneManager.h"
#include <vector>
#include <memory>

#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

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
	template<class T>
	void RemoveComponent();
	//void CustomSendMessage();

	std::string GameObjectName;
	std::string GameObjectID;

	bool enabled = true;
	Renderer renderer;
	Transform transform;

private:
	std::vector<std::shared_ptr<Component>> componentBuffer;
	SceneManager * const sceneManager;
};

template<class T>
inline T* GameObject_v2::AddComponent()
{
	sceneManager->Component_Valid_Test();

	typedef std::vector<std::shared_ptr<Component>>::iterator Component_Iterator;
	Component_Iterator begin = componentBuffer.begin();
	Component_Iterator end = componentBuffer.end();

	//이미 해당 컴포넌트를 가지고 있는지 확인
	for (Component_Iterator iter = begin; iter != end;) {
		T *component_tester = dynamic_cast<T*>(iter->get());
		if (component_tester != NULL) {
			assert("Add Component : This component is already exists!");
		}

		iter++;
	}

	COMPONENT_INIT_DESC compo_desc;
	compo_desc.mGameObj = this;
	compo_desc.mTransform = &transform;
	compo_desc.mTime = sceneManager->Time;
	compo_desc.mRenderer = &renderer;

	T* new_Component = new T;
	new_Component->initialize(compo_desc);
	Component* compo_ptr = dynamic_cast<Component*>(new_Component);
	sceneManager->ClassifyComponent(compo_ptr, this);
	
	return new_Component;
}

template<class T>
inline T* GameObject_v2::GetComponent()
{
	sceneManager->Component_Valid_Test();

	typedef std::vector<std::shared_ptr<Component>>::iterator Component_Iterator;
	Component_Iterator begin = componentBuffer.begin();
	Component_Iterator end = componentBuffer.end();

	//해당 컴포넌트를 가지고 있는지 확인
	for (Component_Iterator iter = begin; iter != end;) {
		T *target_component = dynamic_cast<T*>(iter->get());
		if (target_component != NULL) {
			return target_component;
		}

		iter++;
	}

	assert("GetComponent have Failed. : This Component Doesn't Exists!" && 1 == 0); //여기서 에러 던지니까 아래 코드는 실행x
	return nullptr;
}

template<class T>
inline void GameObject_v2::RemoveComponent()
{
	typedef std::vector<std::shared_ptr<Component>>::iterator Component_Iterator;
	Component_Iterator begin = componentBuffer.begin();
	Component_Iterator end = componentBuffer.end();

	//해당 컴포넌트를 가지고 있는지 확인
	for (Component_Iterator iter = begin; iter != end;) {
		T *target_component = dynamic_cast<T*>(iter->get());
		if (target_component != NULL) {
			componentBuffer.erase(iter);
			sceneManager->Component_Valid_Test();
			return;
		}

		iter++;
	}

	//없는 경우
	MessageBoxA(NULL, "No Component To Remove", "Error", MB_ICONERROR);
	return;
}


