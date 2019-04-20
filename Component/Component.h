#ifndef COMPONENT_H
#define COMPONENT_H
#include "Object.h"

class GameObject_v2; // include 컴파일 주의할것. 아직 포함 안시킨 상태
struct TimeInfo;

class Component abstract : public Object{
	friend class Engine;
public:
	
	//void SendMessage();
	//GameObject_v2 *gameObject;
	
	//Timer *Time;

protected:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();

	GameObject_v2 * gameObject;
	TimeInfo *Time;
};

#endif
