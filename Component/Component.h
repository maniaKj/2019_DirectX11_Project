#ifndef COMPONENT_H
#define COMPONENT_H
#include "Object.h"

class GameObject_v2; // include ������ �����Ұ�. ���� ���� �Ƚ�Ų ����
struct TimeInfo;
class Transform;
class Renderer;

struct COMPONENT_INIT_DESC {
	GameObject_v2 * const gameObj = nullptr;
	TimeInfo * const time = nullptr;
	Transform * const tf = nullptr;
	Renderer * const rd = nullptr;
};

class Component abstract : public Object{
	friend class Engine;
public:
	virtual void initialize(const COMPONENT_INIT_DESC & package) { gameObject = package.gameObj; Time = package.time; }
	//void SendMessage();

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
