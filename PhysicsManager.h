#pragma once
#include <vector>
#include <map>
#include <memory>

class Engine; //include 정의 조심
class GameObject_v2;
class Component;
class Collider;
class Collider_v2;
struct TimeInfo;

namespace reactphysics3d {
	class DynamicsWorld;
	class CollisionBody;
	struct Vector3;
}


class PhysicsManager {
	friend class Engine;
private:
	PhysicsManager(
		Engine * const engine_ptr, 
		TimeInfo * const timeInfo, 
		std::vector<std::shared_ptr<Collider_v2>> * buffer2
	) :
		engine(engine_ptr), 
		Time(timeInfo), 
		collider_v2Buffer(buffer2){}

	~PhysicsManager();

	bool Initialize();
	void PhysicsCompoInit(std::shared_ptr<Collider_v2> _component);
	void Update();
	void PreUpdate();
	void PhysicsUpdate();
	void UpdateCollider();
	void UpdateComponent();
	void CollisionTest();
	void CollisionTest_ver2();
	void react3DCollisioinTest();
	void react3DCollisioinTest(Collider_v2* _collider);

	//std::vector<std::shared_ptr<Collider>> * const colliderBuffer;
	std::vector<std::shared_ptr<Collider_v2>> * const collider_v2Buffer;
	Engine * const engine;
	TimeInfo * const Time;

	reactphysics3d::DynamicsWorld* mReactPhysics_DYNAMIC_WORLD;
	std::map< reactphysics3d::CollisionBody *, Collider_v2*> mBody_Collider_MAP;
	reactphysics3d::Vector3* mGravity;
};