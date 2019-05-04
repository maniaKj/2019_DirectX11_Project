#pragma once
#include "Component.h"
#include "../Graphics/DebugDraw.h" //이거는 가능하면 없애버려야 되는데

namespace MyCustom {
	enum CollisionType { COLLISION_DISJOINT = 0, COLLISION_INTERSECT = 1, COLLISION_CONTAIN = 2 };
	enum COLLIDER { COLLIDER_AABB = 0, COLLIDER_BOX = 1, COLLIDER_SPHERE = 2 };
}

class AABBCollider;
class BoxCollider;
class SphereCollider;
struct DirectX::BoundingBox;

struct COLLIDER_DEBUG_MODEL {
	const DirectX::XMFLOAT3* position;
	const DirectX::XMFLOAT3* rotation;
	const DirectX::XMFLOAT3* scale;

	BYTE typeNum = -1;
	const DirectX::BoundingBox* aabbPtr = nullptr;
	const DirectX::BoundingOrientedBox* boxPtr = nullptr;
	const DirectX::BoundingSphere* spherePtr = nullptr;
	const BoxCollider * boxCollider = nullptr;
};

class Collider : public Component {
	friend class PhysicsManager;
public:
	virtual MyCustom::CollisionType My_CollisionTest(const Collider * collider) abstract;
	virtual DirectX::ContainmentType Debug_CollisionTest(const Collider * collider) abstract;
	virtual void DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color) abstract;
	virtual COLLIDER_DEBUG_MODEL Get_DebugModelType() const abstract;

	MyCustom::COLLIDER type;
	BYTE CollisionMask = 0xff;

	Transform *transform;
protected:
	std::vector<Collider*> collidObj;

private:
	virtual void Update() override {}
};

class AABBCollider : public Collider {
	friend class PhysicsManager;
public:
	void  initialize(const COMPONENT_INIT_DESC & package) override;
	MyCustom::CollisionType My_CollisionTest(const Collider * collider) override;
	DirectX::ContainmentType Debug_CollisionTest(const Collider * collider) override;
	void DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color) override;
	COLLIDER_DEBUG_MODEL Get_DebugModelType() const override;
	

	DirectX::XMFLOAT3 Center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 size = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::BoundingBox colliderModel;
};

class BoxCollider : public Collider {
	friend class PhysicsManager;
	friend class Graphics;
public:
	void  initialize(const COMPONENT_INIT_DESC & package) override;

	MyCustom::CollisionType My_CollisionTest(const Collider * collider) override;
	DirectX::ContainmentType Debug_CollisionTest(const Collider * collider) override;
	void DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color) override;
	COLLIDER_DEBUG_MODEL Get_DebugModelType() const override;

	DirectX::XMFLOAT3 localCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 localScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT3 localRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	DirectX::BoundingOrientedBox colliderModel;

private:
	void Update() override;
	bool Intersect_Test(const BoxCollider* _collider);
	
};

class SphereCollider : public Collider {
	friend class PhysicsManager;
public:
	void  initialize(const COMPONENT_INIT_DESC & package) override;
	MyCustom::CollisionType My_CollisionTest(const Collider * collider) override;
	DirectX::ContainmentType Debug_CollisionTest(const Collider * collider) override;
	void DebugModel_Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::FXMVECTOR color) override;
	COLLIDER_DEBUG_MODEL Get_DebugModelType() const override;

	DirectX::XMFLOAT3 center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);;
	float Radius = 1.0f;
	DirectX::BoundingSphere colliderModel;
};