#include "GameObject_v2.h"

GameObject_v2::GameObject_v2(SceneManager * const sceneM, Model * model, const int & vshaderID, const int & pshaderID, const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3 & rot)
	: sceneManager(sceneM)
{
	renderer.Renderer_Initialize(model, vshaderID, pshaderID);

	COMPONENT_INIT_DESC desc;
	desc.mTransform = &transform;
	desc.mGameObj = this;

	renderer.initialize(desc);

	this->transform.SetPosition(pos);
	this->transform.SetRotation(rot);
}

GameObject_v2::GameObject_v2(GAMEOBJECT_INIT_DESC & desc) :
	sceneManager(desc.scene_manager),
	GameObjectID(*desc.obj_id),
	GameObjectName(*desc.obj_name)
{
	renderer.Renderer_Initialize(desc.model, desc.vshaderID, desc.pshaderID);

	COMPONENT_INIT_DESC c_desc;
	c_desc.mTransform = &transform;
	renderer.initialize(c_desc);

	this->transform.SetPosition(desc.pos);
	this->transform.SetRotation(desc.rot);
}

GameObject_v2::~GameObject_v2()
{
	for (std::vector<std::shared_ptr<Component>>::iterator it = componentBuffer.begin(); it != componentBuffer.end(); it++) {
		//delete it;
	}
}