#pragma once
#include "../Component/ScriptBehaviour.h"

class cstest : public ScriptBehaviour{
public:
	float speed = 1.0f;

	void Update() override;
};