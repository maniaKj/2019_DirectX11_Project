#pragma once
#include <vector>
#include <DirectXMath.h>

class GameObject_v2;
class Collider_v2;

class CollisionInfo {
public:
	GameObject_v2 * mGameObject;
	Collider_v2 * mCollider;
	std::vector<DirectX::XMFLOAT3> mContactPoints;
	std::vector<DirectX::XMVECTOR> mCenterContactVec;
};