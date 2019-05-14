#include "Light_ver2.h"

void DirectionalLight::initialize(const COMPONENT_INIT_DESC& desc)
{
	gameObject = desc.mGameObj;
	Time = desc.mTime;
	transform = desc.mTransform;
}

LIGHT_INFO_DESC DirectionalLight::GetInfoDesc()
{
	LIGHT_INFO_DESC lightDesc = {
		&lightType,
		&transform->GetPositionFloat3(),
		&transform->GetRotationFloat3(),
		&lightColor,
		nullptr,
		nullptr,
		&lightStrength,
		nullptr
	};

	return lightDesc;
}

void SpotLight::initialize(const COMPONENT_INIT_DESC & desc)
{
	gameObject = desc.mGameObj;
	Time = desc.mTime;
	transform = desc.mTransform;
}

LIGHT_INFO_DESC  SpotLight::GetInfoDesc()
{
	LIGHT_INFO_DESC lightDesc = {
		&lightType,
		&transform->GetPositionFloat3(),
		&transform->GetRotationFloat3(),
		&lightColor,
		&Attentuation,
		&Range,
		&lightStrength,
		&SpotAngle
	};

	return lightDesc;
}

void PointLight::initialize(const COMPONENT_INIT_DESC & desc)
{
	gameObject = desc.mGameObj;
	Time = desc.mTime;
	transform = desc.mTransform;
}

LIGHT_INFO_DESC  PointLight::GetInfoDesc()
{
	LIGHT_INFO_DESC lightDesc = {
		&lightType,
		&transform->GetPositionFloat3(),
		&transform->GetRotationFloat3(),
		&lightColor,
		&Attentuation,
		&Range,
		&lightStrength,
		nullptr
	};

	return lightDesc;
}
