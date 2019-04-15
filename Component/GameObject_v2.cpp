#include "GameObject_v2.h"

GameObject_v2::GameObject_v2(SceneManager * const sceneM, Model * model, const int & vshaderID, const int & pshaderID, const XMFLOAT3 & pos, const XMFLOAT3 & rot)
	: sceneManager(sceneM)
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

GameObject_v2::~GameObject_v2()
{
	for (std::vector<Component*>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
		delete *it;
	}
}