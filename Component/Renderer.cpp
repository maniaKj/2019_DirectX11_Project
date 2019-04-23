#include "Renderer.h"

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
	model->Draw(this->transform->worldMatrix, viewProjectionMatrix);
	return;
	if (!drawWireFrame) {

	}
		
	else {
		
	}

}

void Renderer::initialize(const COMPONENT_INIT_DESC & package)
{
	transform = package.tf;
	gameObject = package.gameObj;
	Time = package.time;
}
