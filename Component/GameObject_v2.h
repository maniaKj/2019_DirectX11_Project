#pragma once
#include "Object.h"
#include "Renderer.h"
#include "Transform.h"
#include <vector>

class GameObject_v2 : public Object {
	//friend class Engine;
	//friend class SceneManager; //이따 지울 것
public:
	GameObject_v2(const std::string & modelPath, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot);
	GameObject_v2(const int & modelID, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot);
	GameObject_v2(Model * model, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot);
	bool Initialize(const XMFLOAT3 & pos, const XMVECTOR & rot);
	
	//template<class T>
	//T& AddComponent();
	//template<class T>
	//T& GetComponent();
	//void CustomSendMessage();


	~GameObject_v2();

	Renderer renderer;
	Transform transform;
private:
	std::vector<Component*> componentBuffer;
};