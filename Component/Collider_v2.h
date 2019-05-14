#pragma once
#include "Component.h"
#include "../Graphics/DebugDraw.h" //이거는 가능하면 없애버려야 되는데
#include "CollisionInfo.h"

namespace MyCustom {
	enum COLLIDER_VER2 { COLLIDER_AABB_VER2 = 0, COLLIDER_BOX_VER2 = 1, COLLIDER_SPHERE_VER2 = 2, COLLIDER_CAPSULE_VER2 = 3 };
}

class AABBCollider;
class BoxCollider;
class SphereCollider;
struct DirectX::BoundingBox;
class CollisionInfo;
namespace reactphysics3d {
	class DynamicsWorld;
	class RigidBody;
	class CollisionBody;
	class DynamicsWorld;
	class ProxyShape;
	class BoxShape;
	class SphereShape;
	class CapsuleShape;
	class Transform;
	enum class BodyType;
}

struct COLLIDER_DEBUG_MODEL_VER2 {
	BYTE typeNum = -1;
	DirectX::BoundingOrientedBox mDeubgBox;
	DirectX::BoundingSphere mDebugSphere;
	DirectX::XMFLOAT3 auxVal;
	DirectX::XMFLOAT3 auxVal2;
	//DirectX::Bound
};

class Collider_v2 : public Component {
	friend class PhysicsManager;
public:
	virtual COLLIDER_DEBUG_MODEL_VER2 Get_DebugModelType() const abstract;

	MyCustom::COLLIDER_VER2 type;
	const COMPONENT_TYPE mType = COMPONENT_COLLISION;
	BYTE CollisionMask = 0xff;

	Transform *transform;
	std::vector<std::unique_ptr<CollisionInfo>> mCollisionInfo;
protected:
	virtual reactphysics3d::RigidBody* initialize_React3D(reactphysics3d::DynamicsWorld * _world) abstract;

	reactphysics3d::RigidBody * mRigidBody = nullptr;
	reactphysics3d::ProxyShape * mProxyShape = nullptr;
	reactphysics3d::BodyType * mBodyType = nullptr;

	std::vector<Collider_v2*> collidObj; //중간에 컴포넌트 삭제해서 댕글링 만드는 경우의 수 존재. 근데 스마트 포인터로 바꾸기 귀찮다
	

	void ColliderUpdate();
	void TransformUpdate(reactphysics3d::Transform & _prevTrasnform);
	void BeforePhysicsUpdate();

	bool colllisionTestOn = false;
};

class BoxCollider_ver2 : public Collider_v2 {
	friend class PhysicsManager;
	friend class GraphicsManager;
public:
	void initialize(const COMPONENT_INIT_DESC & desc);
	COLLIDER_DEBUG_MODEL_VER2 Get_DebugModelType() const;
	~BoxCollider_ver2();

	DirectX::XMFLOAT3 localCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 localScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 localRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

private:
	reactphysics3d::RigidBody* initialize_React3D(reactphysics3d::DynamicsWorld * _world) override;

	reactphysics3d::BoxShape * mBoxShape = nullptr;
	DirectX::XMFLOAT3 mExtent;
};

class SphereCollider_ver2 : public Collider_v2 {
	friend class PhysicsManager;
public:
	void initialize(const COMPONENT_INIT_DESC & desc);
	COLLIDER_DEBUG_MODEL_VER2 Get_DebugModelType() const;
	~SphereCollider_ver2();

	DirectX::XMFLOAT3 localCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float Radius = 1.0f;

private:
	reactphysics3d::RigidBody* initialize_React3D(reactphysics3d::DynamicsWorld * _world) override;

	reactphysics3d::SphereShape * mSphereShape = nullptr;
};

class BoxRigidBody : public Collider_v2 {
	friend class PhysicsManager;
	friend class GraphicsManager;
	friend class SceneManager;
public:
	void initialize(const COMPONENT_INIT_DESC & desc) override;
	COLLIDER_DEBUG_MODEL_VER2 Get_DebugModelType() const;
	~BoxRigidBody();

	DirectX::XMFLOAT3 localCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 localScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 localRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

private:
	reactphysics3d::RigidBody* initialize_React3D(reactphysics3d::DynamicsWorld * _world) override;
	void Update() override;

	reactphysics3d::BoxShape * mBoxShape = nullptr;
	DirectX::XMFLOAT3 mExtent;
	reactphysics3d::Transform * mPrevTransform = nullptr;
};

class SphereRigidBody : public Collider_v2 {
	friend class PhysicsManager;
	friend class GraphicsManager;
	friend class SceneManager;
public:
	void initialize(const COMPONENT_INIT_DESC & desc) override;
	COLLIDER_DEBUG_MODEL_VER2 Get_DebugModelType() const;
	~SphereRigidBody();

	DirectX::XMFLOAT3 localCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);;
	float Radius = 1.0f;

private:
	reactphysics3d::RigidBody* initialize_React3D(reactphysics3d::DynamicsWorld * _world) override;
	void Update() override;
	
	reactphysics3d::SphereShape * mSphereShape = nullptr;
	reactphysics3d::Transform * mPrevTransform = nullptr;
};

class CharacterController : public Collider_v2 {
	friend class PhysicsManager;
	friend class GraphicsManager;
	friend class SceneManager;
public:
	void initialize(const COMPONENT_INIT_DESC & desc) override;
	COLLIDER_DEBUG_MODEL_VER2 Get_DebugModelType() const;
	~CharacterController();

	void Move(DirectX::XMFLOAT3 & _motion);
	bool IsGrounded();

	DirectX::XMFLOAT3 localCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	float Radius = 1.0f;
	float Height = 1.0f;

private:
	reactphysics3d::RigidBody* initialize_React3D(reactphysics3d::DynamicsWorld * _world) override;
	void Update() override;

	reactphysics3d::CapsuleShape * mCapsuleShape = nullptr;
	reactphysics3d::Transform * mPrevTransform = nullptr;
};