#include "Component.h"

//GameObject_v2 * Component::GetgameObject()
//{
//	return gameObject;
//}

void Component::initialize(const COMPONENT_INIT_DESC & desc)
{
	gameObject = desc.mGameObj;
	Time = desc.mTime;
}

void Component::Start()
{
}

void Component::Update()
{
}

void Component::FixedUpdate()
{
}
