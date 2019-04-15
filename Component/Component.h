#ifndef COMPONENT_H
#define COMPONENT_H
#include "Object.h"

class GameObject_v2; // include ������ �����Ұ�. ���� ���� �Ƚ�Ų ����
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
