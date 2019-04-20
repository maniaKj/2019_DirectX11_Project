#include "cstest.h"

void cstest::Update()
{
	gameObject->transform.translate(0.0f, speed * Time->deltaTime, 0.0f);
}