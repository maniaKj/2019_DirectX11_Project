#pragma once
#include <cassert>
#include "Behaviour.h"
#include "GameObject_v2.h"
#include "../Timer.h"

class ScriptBehaviour : public Behaviour {
public:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	//void StartCoroutine();

	//bool enable = true;

protected:
	GameObject_v2 * gameObject;
	Timer *Time;
};