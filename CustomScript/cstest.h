#pragma once
#include "../Component/ScriptBehaviour.h"

class cstest : public ScriptBehaviour{
public:
	float speed = 3.0f;

	void Update() override{
		gameObject->transform.translate(0.0f, speed * Time->deltaTime, 0.0f);
		//gameObject->transform.rotate(speed * Time->deltaTime * 0.5f, 0.0f, 0.0f);
	}
};