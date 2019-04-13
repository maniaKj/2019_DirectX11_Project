#pragma once
#include "Object.h"
//#include "GameObject_v2.h"

class Component : public Object{
	friend class Engine;
public:
	//void SendMessage();
	
	//GameObject_v2 * GetgameObject();

protected:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();

	//GameObject_v2 *gameObject;
};