#pragma once
#include "Behaviour.h"


class ScriptBehaviour : public Behaviour {
public:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	//void StartCoroutine();

	//bool enable = true;
	//GameObject_v2 gameObject;
	const COMPONENT_TYPE mType = COMPONENT_SCRIPT;
protected:
	
};

#include "../CustomScript/CustomScriptInclude.h"