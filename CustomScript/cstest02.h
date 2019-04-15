#pragma once
#include "../Component/ScriptBehaviour.h"

class cstest02 : public ScriptBehaviour {
public:
	//cstest cs;
	cstest* tmpcs;

	void Start() override {
		tmpcs = GetComponent<cstest>();
	}

	void Update() override {
		gameObject->transform.rotate(Time->deltaTime * tmpcs->speed, 0.0f, 0.0f);
	}
	
};