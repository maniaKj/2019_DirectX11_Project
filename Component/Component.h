#pragma once
#include "Object.h"

class GameObject_v2; // include 컴파일 주의할것. 아직 포함 안시킨 상태
struct TimeInfo;
class Transform;
class Renderer;
struct ID3D11Device;
struct ID3D11DeviceContext;
template<class T>
class ConstantBuffer;
struct CB_VS_boneData;

enum COMPONENT_TYPE {
	COMPONENT_SCRIPT =		0, 
	COMPONENT_TRANSFORM =	1,
	COMPONENT_RENDERER =	2, 
	COMPONENT_COLLISION =	3, 
	COMPONENT_ANIMATION =	4, 
	COMPONENT_LIGHT =		5,
	COMPONENT_TERRAIN =		6};

struct COMPONENT_INIT_DESC {
	std::string * mComponentID = nullptr;
	GameObject_v2 * mGameObj = nullptr;
	TimeInfo * mTime = nullptr;
	Transform * mTransform = nullptr;
	Renderer * mRenderer = nullptr;
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	ConstantBuffer<CB_VS_boneData> * mBoneData = nullptr;
	//ConstantBuffer<CB_VS_vertexshader> * cb_vs_vertexshader = nullptr;
};

class Component abstract : public Object{
	friend class Engine;
public:
	virtual void initialize(const COMPONENT_INIT_DESC & desc);
	//void SendMessage();

	std::string ComponentID;
	bool enabled = true;
	GameObject_v2 * gameObject;
	TimeInfo *Time;
protected:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();

	
};

#include "BaseComponentInclude.h"

#include <crtdbg.h>
#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
