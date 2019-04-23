#ifndef COMPONENT_H
#define COMPONENT_H
#include "Object.h"

class GameObject_v2; // include 컴파일 주의할것. 아직 포함 안시킨 상태
struct TimeInfo;
class Transform;
class Renderer;

struct COMPONENT_INIT_DESC {
	std::string * componentID;
	GameObject_v2 * gameObj = nullptr;
	TimeInfo * time = nullptr;
	Transform * tf = nullptr;
	Renderer * rd = nullptr;
};

class Component abstract : public Object{
	friend class Engine;
public:
	virtual void initialize(const COMPONENT_INIT_DESC & package) { gameObject = package.gameObj; Time = package.time; }
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
#endif
