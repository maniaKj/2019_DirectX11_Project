#pragma once
#include "../Component/ScriptBehaviour.h"

class cstest02 : public ScriptBehaviour {
public:
	//cstest cs;
	cstest* tmpcs;
	float test = 1.0f;

	void Start() override;

	void Update() override;
	
};