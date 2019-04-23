#ifndef RENDERER_H
#define RENDERER_H
#include "Component.h"
#include "../Graphics/Model.h"
#include "../Graphics/Shaders.h"
#include <string>

class Renderer : public Component {
public:
	Renderer() {}
	bool Renderer_Initialize(Model * model_ptr, const int & vshader_id, const int & pshader_id);
	void Renderer_Draw(const DirectX::XMMATRIX & viewProjectionMatrix);
	void initialize(const COMPONENT_INIT_DESC & package) override;
	void SetModel(Model * _model);

	//Properties
	bool enabled = true;
	int modelID = -1;
	int vertexshaderID = -1, pixelshaderID= -1;
	std::string model_filepath;

	bool drawWireFrame = false;
	
private:
	Model* model;
	Transform *transform;
};

#endif