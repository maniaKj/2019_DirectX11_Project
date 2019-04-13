#pragma once
#include "../Component/ScriptBehaviour.h"

class testScript01 : public ScriptBehaviour {
	float speed = 0.1f;
	float rotation_term = 3.0f;

	void Update();
};