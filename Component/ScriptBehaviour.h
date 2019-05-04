#pragma once
#include "Behaviour.h"


class ScriptBehaviour abstract : public Behaviour {
public:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	//void StartCoroutine();

	//bool enable = true;
	//GameObject_v2 gameObject;
protected:
	
};

#include "../CustomScript/CustomScriptInclude.h"