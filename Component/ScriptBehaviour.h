#pragma once
#include "Behaviour.h"


class ScriptBehaviour abstract : public Behaviour {
public:
	void initialize(GameObject_v2 *gameObj, TimeInfo *time) { gameObject = gameObj; Time = time; }
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	//void StartCoroutine();

	//bool enable = true;
	//GameObject_v2 gameObject;
protected:
	
};

#include "../CustomScript/CustomScriptInclude.h"