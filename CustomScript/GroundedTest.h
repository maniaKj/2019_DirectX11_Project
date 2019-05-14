#pragma once
#include "../Component/ScriptBehaviour.h"

class GroundTest : public ScriptBehaviour {
public:
	CharacterController * chracterController = nullptr;
	bool isGrounded = false;

	void Start() override;

	void Update() override;

};