#include "Collider.h"

using DirectX::operator +=;
using DirectX::operator +;

void AABBCollider::initialize(const COMPONENT_INIT_DESC & package)
{
	type = MyCustom::COLLIDER_AABB;
	gameObject = package.gameObj;
	transform = package.tf;
	Time = package.time;
	//ComponentID = *package.componentID;
}

MyCustom::CollisionType AABBCollider::My_CollisionTest(const Collider * collider)
{
	return MyCustom::COLLISION_DISJOINT;
}

DirectX::ContainmentType AABBCollider::Debug_CollisionTest(const Collider * collider)
{
	return DirectX::DISJOINT;
}


//DirectX::ContainmentType AABBCollider::Debug_CollisionTest(const Collider * collider)
//{
//	if (CollisionMask & collider->CollisionMask == 0) return DirectX::ContainmentType::DISJOINT;
//
//	
//	return DirectX::ContainmentType();
//}

void AABBCollider::DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color)
{
}

COLLIDER_DEBUG_MODEL AABBCollider::Get_DebugModelType() const
{
	COLLIDER_DEBUG_MODEL desc;
	desc.position = &transform->GetPositionFloat3();
	desc.rotation = &transform->GetRotationFloat3();
	desc.scale = &transform->GetScaleFloat3();

	desc.typeNum = 0;
	desc.aabbPtr = &debugModel;
	return desc;
}

void BoxCollider::initialize(const COMPONENT_INIT_DESC & package)
{
	type = MyCustom::COLLIDER_BOX;
	gameObject = package.gameObj;
	transform = package.tf;
	Time = package.time;
	//ComponentID = *package.componentID;
}

void BoxCollider::Update()
{
	collidObj.clear();

	DirectX::XMFLOAT3 tmp = transform->GetPositionFloat3();
	debugModel.Center = DirectX::XMFLOAT3(tmp.x + center.x, tmp.y + center.y, tmp.z + center.z);

	tmp = transform->GetRotationFloat3();
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(tmp.x, tmp.y, tmp.z);

	DirectX::XMStoreFloat4(&debugModel.Orientation, quaternion);
}

MyCustom::CollisionType BoxCollider::My_CollisionTest(const Collider * collider)
{
	if (CollisionMask & collider->CollisionMask == 0) return MyCustom::COLLISION_DISJOINT;



	return MyCustom::COLLISION_DISJOINT;
}

DirectX::ContainmentType BoxCollider::Debug_CollisionTest(const Collider * collider)
{
	if(CollisionMask & collider->CollisionMask == 0) return DirectX::DISJOINT;

	switch (collider->type) {
	case MyCustom::COLLIDER_BOX :
		COLLIDER_DEBUG_MODEL desc = collider->Get_DebugModelType();
		return debugModel.Contains(*desc.boxPtr);
		//return desc.boxPtr->Contains(debugModel);
		break;
	}
	
	return DirectX::DISJOINT;
}




void BoxCollider::DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color)
{
}

COLLIDER_DEBUG_MODEL BoxCollider::Get_DebugModelType() const
{
	COLLIDER_DEBUG_MODEL desc;
	desc.position = &transform->GetPositionFloat3();
	desc.rotation = &transform->GetRotationFloat3();
	desc.scale = &transform->GetScaleFloat3();
	
	desc.typeNum = 1;
	desc.boxPtr = &debugModel;
	return desc;
}

void SphereCollider::initialize(const COMPONENT_INIT_DESC & package)
{
	type = MyCustom::COLLIDER_SPHERE;
	gameObject = package.gameObj;
	transform = package.tf;
	Time = package.time;
	//ComponentID = *package.componentID;
}

MyCustom::CollisionType SphereCollider::My_CollisionTest(const Collider * collider)
{
	return MyCustom::COLLISION_DISJOINT;
}

DirectX::ContainmentType SphereCollider::Debug_CollisionTest(const Collider * collider)
{
	return DirectX::DISJOINT;
}



void SphereCollider::DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color)
{
}

COLLIDER_DEBUG_MODEL SphereCollider::Get_DebugModelType() const
{
	COLLIDER_DEBUG_MODEL desc;
	desc.position = &transform->GetPositionFloat3();
	desc.rotation = &transform->GetRotationFloat3();
	desc.scale = &transform->GetScaleFloat3();

	desc.typeNum = 2;
	desc.spherePtr = &debugModel;
	return desc;
}
