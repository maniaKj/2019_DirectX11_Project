#include "GroundedTest.h"

void GroundTest::Start()
{
	chracterController = GetComponent<CharacterController>();
}

void GroundTest::Update()
{
	isGrounded = chracterController->IsGrounded();

	if (!isGrounded) {
		gameObject->transform.translate(0.0f, -40.0f * Time->GetDeltaTime(), 0.0f);
	}

	if (InputKeyboard->KeyIsPressed('T')) {
		gameObject->transform.translate(0.0f, 20.0f * Time->GetDeltaTime(), 0.0f);
	}

	if (InputKeyboard->KeyIsPressed('J')) {
		gameObject->transform.translate(15.0f * Time->GetDeltaTime(), 0.0f, 0.0f);
	}
}
