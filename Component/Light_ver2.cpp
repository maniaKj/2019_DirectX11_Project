#include "Light_ver2.h"

void DirectionalLight::initialize(const COMPONENT_INIT_DESC& package)
{
	gameObject = package.gameObj;
	Time = package.time;
	transform = package.tf;
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

void SpotLight::initialize(const COMPONENT_INIT_DESC & package)
{
	gameObject = package.gameObj;
	Time = package.time;
	transform = package.tf;
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

void PointLight::initialize(const COMPONENT_INIT_DESC & package)
{
	gameObject = package.gameObj;
	Time = package.time;
	transform = package.tf;
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
