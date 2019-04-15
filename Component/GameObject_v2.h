#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "Object.h"
#include "Renderer.h" //transform, Renderer, Ŀ���� ��ũ��Ʈ �ϳ��� ����
#include "Transform.h"
#include "../SceneManager.h"
#include <vector>

class GameObject_v2 : public Object {
	friend class Engine;
public:
	GameObject_v2(SceneManager * const sceneM,  Model * model, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot);
	bool Initialize(const XMFLOAT3 & pos, const XMVECTOR & rot);
	
	template<class T>
	T* AddComponent();
	template<class T>
	T* GetComponent();
	//void CustomSendMessage();


	~GameObject_v2();

	Renderer renderer;
	Transform transform;
private:
	std::vector<Component*> componentBuffer;
	SceneManager * const sceneManager;
};

template<class T>
inline T* GameObject_v2::AddComponent()
{
	T *tmp_class = new T;
	tmp_class->initialize(this, sceneManager->Time); //���߿� ��ũ�� ������ Ȱ��
	Component * component = dynamic_cast<Component*>(tmp_class); // T�� Component�� �� ����ó��, ������Ʈ�� ����ȯ ������ ����

	std::string s = "asdf"; //���߿� ������Ʈ �̸� ��, ������Ʈ �̸� �߰��ؼ� ����
	assert("Add Component have Failed." && component != NULL);

	componentBuffer.push_back(component);

	return tmp_class;
}

template<class T>
inline T* GameObject_v2::GetComponent()
{
	//T *tmp_class = new T();
	//Component *tmp_compo = dynamic_cast<Component*>(tmp_class);
	//if (dynamic_cast<Component*>(tmp_class) == NULL) {

	//}
	//std::string s = "asdf"; //���߿� ������Ʈ �̸� ��, ������Ʈ �̸� �߰��ؼ� ����
	//assert("Get Component have Failed." + s && tmp_compo != NULL);
	//delete tmp_class;

	for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
		T *tmp = dynamic_cast<T*>(*it);
		if (tmp != NULL) {
			return tmp;
		}
	}

	assert("GetComponent have Failed." && 1 == 0); //���⼭ ���� �����ϱ� �Ʒ� �ڵ�� ����x
	return nullptr;
}

#endif

