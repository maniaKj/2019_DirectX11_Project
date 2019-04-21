#include "Renderer.h"

void Renderer::SetTransform(Transform & tf)
{
	transform = &tf;
}

Renderer::Renderer(const std::string & modelPath, const int & vshaderID, const int & pshaderID)
{
	model_filepath = modelPath;
	vertexshaderID = vshaderID;
	pixelshaderID = pshaderID;
}

bool Renderer::Renderer_Initialize(const int & model_id, const int & vshader_id, const int & pshader_id)
{
	modelID = model_id;
	vertexshaderID = vshader_id;
	pixelshaderID = pshader_id;

	

	return true;
}

bool Renderer::Renderer_Initialize(Model * model_ptr, const int & vshader_id, const int & pshader_id)
{
	model = model_ptr;
	vertexshaderID = vshader_id;
	pixelshaderID = pshader_id;

	return true;
}

bool Renderer::Renderer_Initialize(const std::string & modelPath, const int & vshaderID, const int & pshaderID)
{
	model_filepath = modelPath;
	vertexshaderID = vshaderID;
	pixelshaderID = pshaderID;

	return true;
}

bool Renderer::Renderer_Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	//¸ðµ¨ ÃÊ±âÈ­
	if (!model->Initialize(model_filepath, device, deviceContext, cb_vs_vertexshader)) {
		ErrorLogger::Log("test model Initialize Failed");
		return false;
	}

	return true;
}

bool Renderer::Renderer_Initialize(const std::string & modelPath, const int & vshaderID, const int & pshaderID, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	model_filepath = modelPath;
	vertexshaderID = vshaderID;
	pixelshaderID = pshaderID;

	//¸ðµ¨ ÃÊ±âÈ­
	if (!model->Initialize(model_filepath, device, deviceContext, cb_vs_vertexshader)) {
		ErrorLogger::Log("test model Initialize Failed");
		return false;
	}

	return true;
}

void Renderer::Renderer_Draw(const DirectX::XMMATRIX & viewProjectionMatrix)
{
	if(enabled)
	model->Draw(this->transform->worldMatrix, viewProjectionMatrix);
}