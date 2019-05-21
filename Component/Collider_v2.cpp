#include "Collider_v2.h"
#include <reactphysics3d/reactphysics3d.h>

using DirectX::operator +=;
using DirectX::operator +;

BoxCollider_ver2::BoxCollider_ver2(const COMPONENT_INIT_DESC & desc) : Collider_v2(desc)
{
	std::strcpy(mComponentName, "BoxCollider");
	colllisionTestOn = true;
}

reactphysics3d::RigidBody* BoxCollider_ver2::initialize_React3D(reactphysics3d::DynamicsWorld * _world)
{
	reactphysics3d::Vector3 pos(transform->position.x, transform->position.y, transform->position.z);
	reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	reactphysics3d::Transform t_form(pos, quat);

	mRigidBody = _world->createRigidBody(t_form);
	mRigidBody->setType(reactphysics3d::BodyType::STATIC);

	pos = reactphysics3d::Vector3(localCenter.x, localCenter.y, localCenter.z);
	quat = reactphysics3d::Quaternion::fromEulerAngles(localRotation.x, localRotation.y, localRotation.z);
	t_form = reactphysics3d::Transform(pos, quat);

	DirectX::XMFLOAT3 _scale = transform->GetScaleFloat3();
	mExtent = DirectX::XMFLOAT3(_scale.x * localScale.x, _scale.y * localScale.y, _scale.z * localScale.z);
	reactphysics3d::Vector3 extent(mExtent.x, mExtent.y, mExtent.z);

	mBoxShape = new reactphysics3d::BoxShape(extent);
	mProxyShape = mRigidBody->addCollisionShape(mBoxShape, t_form, 1.0f);

	return mRigidBody;
}

COLLIDER_DEBUG_MODEL_VER2 BoxCollider_ver2::Get_DebugModelType() const
{
	reactphysics3d::Transform t_form = mRigidBody->getTransform();
	reactphysics3d::Vector3 pos = t_form.getPosition();
	reactphysics3d::Quaternion quat = t_form.getOrientation();

	DirectX::XMFLOAT3 Dpos(pos.x, pos.y, pos.z);
	DirectX::XMFLOAT4 Dquat(quat.x, quat.y, quat.z, quat.w);

	COLLIDER_DEBUG_MODEL_VER2 desc;
	desc.typeNum = MyCustom::COLLIDER_VER2::COLLIDER_BOX_VER2;
	desc.mDeubgBox = DirectX::BoundingOrientedBox(Dpos, mExtent, Dquat);
	return desc;
}

BoxCollider_ver2::~BoxCollider_ver2()
{
	delete mBoxShape;
}

SphereCollider_ver2::SphereCollider_ver2(const COMPONENT_INIT_DESC & desc) : Collider_v2(desc) {
	std::strcpy(mComponentName, "Sphere Collider");
	colllisionTestOn = true;
}

COLLIDER_DEBUG_MODEL_VER2 SphereCollider_ver2::Get_DebugModelType() const
{
	reactphysics3d::Transform t_form = mRigidBody->getTransform();
	reactphysics3d::Vector3 pos = t_form.getPosition();

	DirectX::XMFLOAT3 Dpos(pos.x, pos.y, pos.z);

	COLLIDER_DEBUG_MODEL_VER2 desc;
	desc.typeNum = MyCustom::COLLIDER_VER2::COLLIDER_SPHERE_VER2;
	desc.mDebugSphere = DirectX::BoundingSphere(Dpos, Radius);
	return desc;
}

SphereCollider_ver2::~SphereCollider_ver2()
{
	delete mSphereShape;
}

reactphysics3d::RigidBody* SphereCollider_ver2::initialize_React3D(reactphysics3d::DynamicsWorld * _world)
{
	reactphysics3d::Vector3 pos(transform->position.x, transform->position.y, transform->position.z);
	reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	reactphysics3d::Transform t_form(pos, quat);

	mRigidBody = _world->createRigidBody(t_form);
	mRigidBody->setType(reactphysics3d::BodyType::STATIC);

	pos = reactphysics3d::Vector3(localCenter.x, localCenter.y, localCenter.z);
	quat = reactphysics3d::Quaternion::identity();
	t_form = reactphysics3d::Transform(pos, quat);

	DirectX::XMFLOAT3 _scale = transform->GetScaleFloat3();

	mSphereShape = new reactphysics3d::SphereShape(Radius);
	mProxyShape = mRigidBody->addCollisionShape(mSphereShape, t_form, 1.0f);

	return mRigidBody;
}

BoxRigidBody::BoxRigidBody(const COMPONENT_INIT_DESC & desc) : Collider_v2(desc)
{
	std::strcpy(mComponentName, "Box RigidBody");
}

COLLIDER_DEBUG_MODEL_VER2 BoxRigidBody::Get_DebugModelType() const
{
	reactphysics3d::Transform t_form = mRigidBody->getTransform();
	reactphysics3d::Vector3 pos = t_form.getPosition();
	reactphysics3d::Quaternion quat = t_form.getOrientation();

	DirectX::XMFLOAT3 Dpos(pos.x, pos.y, pos.z);
	DirectX::XMFLOAT4 Dquat(quat.x, quat.y, quat.z, quat.w);

	COLLIDER_DEBUG_MODEL_VER2 desc;
	desc.typeNum = MyCustom::COLLIDER_VER2::COLLIDER_BOX_VER2;
	desc.mDeubgBox = DirectX::BoundingOrientedBox(Dpos, mExtent, Dquat);
	return desc;
}

BoxRigidBody::~BoxRigidBody()
{
	delete mBoxShape;
	delete mPrevTransform;
}

reactphysics3d::RigidBody* BoxRigidBody::initialize_React3D(reactphysics3d::DynamicsWorld * _world)
{
	reactphysics3d::Vector3 pos(transform->position.x, transform->position.y, transform->position.z);
	reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	reactphysics3d::Transform t_form(pos, quat);

	mRigidBody = _world->createRigidBody(t_form);
	mRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);

	mPrevTransform = new reactphysics3d::Transform(t_form);

	pos = reactphysics3d::Vector3(localCenter.x, localCenter.y, localCenter.z);
	quat = reactphysics3d::Quaternion::fromEulerAngles(localRotation.x, localRotation.y, localRotation.z);
	t_form = reactphysics3d::Transform(pos, quat);

	DirectX::XMFLOAT3 _scale = transform->GetScaleFloat3();
	mExtent = DirectX::XMFLOAT3(_scale.x * localScale.x, _scale.y * localScale.y, _scale.z * localScale.z);
	reactphysics3d::Vector3 extent(mExtent.x, mExtent.y, mExtent.z);

	mBoxShape = new reactphysics3d::BoxShape(extent);
	mProxyShape = mRigidBody->addCollisionShape(mBoxShape, t_form, 1.0f);

	return mRigidBody;
}

void BoxRigidBody::Update()
{
	TransformUpdate(*mPrevTransform);
}

Collider_v2::Collider_v2(const COMPONENT_INIT_DESC & desc) : Component(desc), transform(desc.mTransform) {
	mComponentType = COMPONENT_COLLISION;
}

MyCustom::COLLIDER_VER2 Collider_v2::getColliderType()
{
	return mColliderType;
}

void Collider_v2::ColliderUpdate()
{
	reactphysics3d::Vector3 pos(transform->position.x, transform->position.y, transform->position.z);
	reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	reactphysics3d::Transform t_form(pos, quat);
	mRigidBody->setTransform(t_form);
}

void Collider_v2::TransformUpdate(reactphysics3d::Transform & _prevTrasnform)
{
	reactphysics3d::Transform t_form = mRigidBody->getTransform();
	reactphysics3d::Transform lerpedTransform = reactphysics3d::Transform::interpolateTransforms(_prevTrasnform, t_form, 0.2);
	_prevTrasnform = lerpedTransform;

	reactphysics3d::Vector3 pos = lerpedTransform.getPosition();
	reactphysics3d::Quaternion quat = lerpedTransform.getOrientation();
	DirectX::XMVECTOR quatVec = DirectX::XMVectorSet(quat.x, quat.y, quat.z, quat.w);

	transform->SetPosition(pos.x, pos.y, pos.z);
	transform->SetRotation(quatVec);
	//transform->rotate(60.0f, 60.0f, 60.0f);
}

void Collider_v2::BeforePhysicsUpdate()
{
	mCollisionInfo.clear();
	if (transform->TRANSFORM_UPDATED) ColliderUpdate();
}

SphereRigidBody::SphereRigidBody(const COMPONENT_INIT_DESC & desc) : Collider_v2(desc)
{
	std::strcpy(mComponentName, "Sphere RigidBody");
}

COLLIDER_DEBUG_MODEL_VER2 SphereRigidBody::Get_DebugModelType() const
{
	reactphysics3d::Transform t_form = mRigidBody->getTransform();
	reactphysics3d::Vector3 pos = t_form.getPosition();

	DirectX::XMFLOAT3 Dpos(pos.x, pos.y, pos.z);

	COLLIDER_DEBUG_MODEL_VER2 desc;
	desc.typeNum = MyCustom::COLLIDER_VER2::COLLIDER_SPHERE_VER2;
	desc.mDebugSphere = DirectX::BoundingSphere(Dpos, Radius);
	return desc;
}

SphereRigidBody::~SphereRigidBody()
{
	delete mSphereShape;
	delete mPrevTransform;
}

reactphysics3d::RigidBody* SphereRigidBody::initialize_React3D(reactphysics3d::DynamicsWorld * _world)
{
	reactphysics3d::Vector3 pos(transform->position.x, transform->position.y, transform->position.z);
	reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	reactphysics3d::Transform t_form(pos, quat);

	mRigidBody = _world->createRigidBody(t_form);
	mRigidBody->setType(reactphysics3d::BodyType::DYNAMIC);

	mPrevTransform = new reactphysics3d::Transform(t_form);

	pos = reactphysics3d::Vector3(localCenter.x, localCenter.y, localCenter.z);
	quat = reactphysics3d::Quaternion::identity();
	t_form = reactphysics3d::Transform(pos, quat);

	DirectX::XMFLOAT3 _scale = transform->GetScaleFloat3();

	mSphereShape = new reactphysics3d::SphereShape(Radius);
	mProxyShape = mRigidBody->addCollisionShape(mSphereShape, t_form, 1.0f);

	return mRigidBody;
}

void SphereRigidBody::Update()
{
	TransformUpdate(*mPrevTransform);
}

CharacterController::CharacterController(const COMPONENT_INIT_DESC & desc) : Collider_v2(desc)
{
	std::strcpy(mComponentName, "Capsule Collider");
	colllisionTestOn = true;
}

COLLIDER_DEBUG_MODEL_VER2 CharacterController::Get_DebugModelType() const
{
	reactphysics3d::Transform t_form = mRigidBody->getTransform();
	reactphysics3d::Vector3 pos = t_form.getPosition();
	reactphysics3d::Quaternion quat = t_form.getOrientation();
	reactphysics3d::Quaternion upquat = reactphysics3d::Quaternion(0.0f, 1.0f, 0.0f, 0.0f);

	upquat = quat * upquat * quat.getConjugate();
	reactphysics3d::Vector3 upVec = upquat.getVectorV();
	DirectX::XMVECTOR upDxVec = DirectX::XMVectorSet(upVec.x, upVec.y, upVec.z, 0.0f);
	DirectX::XMVector3Normalize(upDxVec);

	float HeightVal = Height / 2;
	DirectX::XMFLOAT3 Dpos(pos.x, pos.y, pos.z);
	DirectX::XMVECTOR DxposVec = DirectX::XMLoadFloat3(&Dpos);
	DirectX::XMVECTOR capsuleSpherePos = DirectX::XMVectorAdd(DxposVec, DirectX::XMVectorMultiply(upDxVec, DirectX::XMVectorSet(HeightVal, HeightVal, HeightVal, 0.0f)));
	DirectX::XMStoreFloat3(&Dpos, capsuleSpherePos);

	COLLIDER_DEBUG_MODEL_VER2 desc;
	desc.typeNum = MyCustom::COLLIDER_VER2::COLLIDER_CAPSULE_VER2;
	desc.mDebugSphere = DirectX::BoundingSphere(Dpos, Radius);

	HeightVal = Height * -1;
	capsuleSpherePos = DirectX::XMVectorAdd(DxposVec, DirectX::XMVectorMultiply(upDxVec, DirectX::XMVectorSet(HeightVal, HeightVal, HeightVal, 0.0f)));
	DirectX::XMStoreFloat3(&Dpos, capsuleSpherePos);
	desc.auxVal = Dpos;

	if (mCollisionInfo.size() != 0) {
		desc.auxVal2 = mCollisionInfo[0]->mContactPoints[0];
	}
	return desc;
}

CharacterController::~CharacterController()
{
	delete mCapsuleShape;
	delete mPrevTransform;
}

void CharacterController::Move(DirectX::XMFLOAT3 & _motion)
{
	
}

bool CharacterController::IsGrounded()
{
	for (std::vector<std::unique_ptr<CollisionInfo>>::iterator iter = mCollisionInfo.begin(); iter != mCollisionInfo.end(); iter++) {
		std::vector<DirectX::XMVECTOR> * contactPoints = &(*iter)->mCenterContactVec;
		for (std::vector<DirectX::XMVECTOR>::iterator it2 = contactPoints->begin(); it2 != contactPoints->end(); it2++) {
			if (DirectX::XMVectorGetY(*it2) < -0.5f) {
				return true;
			}
		}
	}
	return false;
}

reactphysics3d::RigidBody* CharacterController::initialize_React3D(reactphysics3d::DynamicsWorld * _world)
{
	reactphysics3d::Vector3 pos(transform->position.x, transform->position.y, transform->position.z);
	reactphysics3d::Quaternion quat = reactphysics3d::Quaternion::fromEulerAngles(transform->rotation.x, transform->rotation.y, transform->rotation.z);
	reactphysics3d::Transform t_form(pos, quat);

	mRigidBody = _world->createRigidBody(t_form);
	mRigidBody->setType(reactphysics3d::BodyType::STATIC);

	mPrevTransform = new reactphysics3d::Transform(t_form);

	pos = reactphysics3d::Vector3(localCenter.x, localCenter.y, localCenter.z);
	quat = reactphysics3d::Quaternion::identity();
	t_form = reactphysics3d::Transform(pos, quat);

	DirectX::XMFLOAT3 _scale = transform->GetScaleFloat3();

	mCapsuleShape = new reactphysics3d::CapsuleShape(Radius, Height);
	mProxyShape = mRigidBody->addCollisionShape(mCapsuleShape, t_form, 1.0f);

	return mRigidBody;
}

void CharacterController::Update()
{
	TransformUpdate(*mPrevTransform);
}
