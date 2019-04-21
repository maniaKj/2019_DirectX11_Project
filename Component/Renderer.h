#ifndef RENDERER_H
#define RENDERER_H
#include "Component.h"
#include "../Graphics/Model.h"
#include "../Graphics/Shaders.h"
#include <string>

class Renderer : public Component {
public:
	Renderer() {}
	Renderer(const std::string & modelPath, const int & vshaderID, const int & pshaderID);
	bool Renderer_Initialize(const int & model_id, const int & vshader_id, const int & pshader_id);
	bool Renderer_Initialize(Model * model_ptr, const int & vshader_id, const int & pshader_id);
	bool Renderer_Initialize(const std::string & modelPath, const int & vshaderID, const int & pshaderID);
	bool Renderer_Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	bool Renderer_Initialize(const std::string & modelPath, const int & vshaderID, const int & pshaderID, ID3D11Device * device, ID3D11DeviceContext * deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader);
	void Renderer_Draw(const DirectX::XMMATRIX & viewProjectionMatrix);
	void SetTransform(Transform &tf);

	//Properties
	bool enabled = true;
	int modelID = -1;
	int vertexshaderID = -1, pixelshaderID= -1;
	std::string model_filepath;
	
private:
	Model* model;
	Transform *transform;
};

#endif