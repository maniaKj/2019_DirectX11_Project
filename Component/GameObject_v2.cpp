#include "GameObject_v2.h"

GameObject_v2::GameObject_v2(const std::string & modelPath, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot)
{
	renderer.Renderer_Initialize(modelPath, vshaderID, pshaderID);
	renderer.SetTransform(transform);

	this->transform.SetPosition(pos);
	this->transform.SetRotation(rot);
}

GameObject_v2::GameObject_v2(const int & modelID, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot)
{
	//renderer.Renderer_Initialize(modelPath, vshaderID, pshaderID);
	renderer.SetTransform(transform);

	this->transform.SetPosition(pos);
	this->transform.SetRotation(rot);
}

GameObject_v2::GameObject_v2(Model * model, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot)
{
	renderer.Renderer_Initialize(model, vshaderID, pshaderID);
	renderer.SetTransform(transform);

	this->transform.SetPosition(pos);
	this->transform.SetRotation(rot);
}

bool GameObject_v2::Initialize(const XMFLOAT3 & pos, const XMVECTOR & rot)
{
	renderer.SetTransform(transform);

	this->transform.SetPosition(pos);
	this->transform.SetRotation(rot);
	return true;
}

//template<class T>
//T& GameObject_v2::AddComponent()
//{
//	T *tmp_class = new T();
//	Component * component = dynamic_cast<Component*>(tmp_class);
//
//	std::string s = "asdf"; //나중에 오브젝트 이름 및, 컴포넌트 이름 추가해서 삽입
//	assert("Add Component have Failed." + s && component != NULL);
//
//	componentBuffer.push_back(component);
//	
//	return *tmp_class;
//}
//
//template<class T>
//T& GameObject_v2::GetComponent()
//{
//	T *tmp_class = new T();
//	Component *tmp_compo = dynamic_cast<Component*>(tmp_class);
//
//	std::string s = "asdf"; //나중에 오브젝트 이름 및, 컴포넌트 이름 추가해서 삽입
//	assert("Get Component have Failed." + s && tmp_compo != NULL);
//	delete tmp_class;
//
//	for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
//		T *tmp = dynamic_cast<T>(*it);
//		if (tmp != NULL) {
//			return *tmp;
//		}
//	}
//}

GameObject_v2::~GameObject_v2()
{
	for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
		delete *it;
	}
}