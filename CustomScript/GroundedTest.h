#pragma once
#include "../Component/ScriptBehaviour.h"

class GroundTest : public ScriptBehaviour {
public:
	NEVER_USE_CONSTRUCTOR(GroundTest);

	CharacterController * chracterController = nullptr;
	bool isGrounded = false;

	void Start() override;

	void Update() override;

};