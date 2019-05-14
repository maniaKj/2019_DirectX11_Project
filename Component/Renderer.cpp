#include "Renderer.h"
#include "Animator.h"

void Renderer::SetModel(Model * _model)
{
	model = _model;
}

bool Renderer::Renderer_Initialize(Model * model_ptr, const int & vshader_id, const int & pshader_id)
{
	model = model_ptr;
	vertexshaderID = vshader_id;
	pixelshaderID = pshader_id;

	return true;
}

void Renderer::Renderer_Draw(const DirectX::XMMATRIX & viewProjectionMatrix)
{
	if(!drawSkinnedMesh)
		model->Draw(this->transform->worldMatrix, viewProjectionMatrix);
	else
		model->Draw_skinnedMesh(this->transform->worldMatrix, viewProjectionMatrix, animator);
	return;
	if (!drawWireFrame) {

	}
		
	else {
		
	}

}

void Renderer::initialize(const COMPONENT_INIT_DESC & desc)
{
	gameObject = desc.mGameObj;
	Time = desc.mTime;
	transform = desc.mTransform;
}
