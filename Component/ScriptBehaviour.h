#pragma once
#include "Behaviour.h"
#include "../Keyboard/KeyboardClass.h"
#include "../Mouse/MouseClass.h"

class ScriptBehaviour : public Behaviour {
	friend class ScriptBehaviourManager;
public:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	//void StartCoroutine();

	//bool enable = true;
	//GameObject_v2 gameObject;
	const COMPONENT_TYPE mType = COMPONENT_SCRIPT;
protected:
	KeyboardClass * InputKeyboard = nullptr;
	MouseClass * InputMouse = nullptr;
private:
};

#include "../CustomScript/CustomScriptInclude.h"