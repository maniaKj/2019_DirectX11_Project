#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object.h"
#include "Component.h"
#include "../SceneManager.h"
#include <vector>

struct GAMEOBJECT_INIT_DESC {
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
	GameObject_v2(SceneManager * const sceneM,  Model * model, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot);
	
	template<class T>
	T* AddComponent();
	template<class T>
	T* GetComponent();
	//void CustomSendMessage();


	~GameObject_v2();
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
	{//이미 해당 컴포넌트를 가지고 있는지 확인
		for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
			T *tmp = dynamic_cast<T*>(*it);
			if (tmp != NULL) {
				assert("Add Component : This component is already exists!");
			}
		}
	}

	T *tmp_class = new T;
	const COMPONENT_INIT_DESC compo_desc = { this, sceneManager->Time, &transform, &renderer };
	tmp_class->initialize(compo_desc); //나중에 매크로 생성자 활용
	Component * component = dynamic_cast<Component*>(tmp_class); // T가 Component일 때 예외처리, 컴포넌트에 형변환 연산자 삽입

	std::string s = "asdf"; //나중에 오브젝트 이름 및, 컴포넌트 이름 추가해서 삽입
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

	assert("GetComponent have Failed. : This Component Doesn't Exists!" && 1 == 0); //여기서 에러 던지니까 아래 코드는 실행x
	return nullptr;
}

#endif

