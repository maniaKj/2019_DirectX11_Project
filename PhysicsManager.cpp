#include "PhysicsManager.h"
#include "Timer.h"
#include "Component/GameObject_v2.h"
#include "Component/CollisionInfo.h"
#include <cassert>

//#pragma comment (lib, "reactphysics3d.lib")
#include <reactphysics3d/reactphysics3d.h>
#include <reactphysics3d/collision/ContactManifold.h>
#include <reactphysics3d/constraint/ContactPoint.h>

class r3dCallback : public reactphysics3d::CollisionCallback {
public:
	int aa = 0;
	void notifyContact(const CollisionCallbackInfo &collisionCallbackInfo) {
		reactphysics3d::ContactManifoldListElement * t = collisionCallbackInfo.contactManifoldElements;
		reactphysics3d::ContactManifold * maf = t->getContactManifold();
		//maf->getNext();
		reactphysics3d::ContactPoint * point = maf->getContactPoints();
		reactphysics3d::Vector3 contactpoint1 =  point->getLocalPointOnShape1();
		//collisionCallbackInfo.contactManifoldElements[0]->
		aa++;
	}
};

class r3dCollisionCallback : public reactphysics3d::CollisionCallback {
public:
	r3dCollisionCallback(Collider_v2 * _collider, std::map< reactphysics3d::CollisionBody *, Collider_v2*> * _map) :
		mCollider(_collider) ,
		mBody_Collider_MAP(_map) {}

	void ContactManifoldTraverse(reactphysics3d::ContactManifoldListElement * _maniFoldList) {
		reactphysics3d::ContactManifold * contactManifold = _maniFoldList->getContactManifold();

		while (contactManifold != nullptr) {
			Make_CollisionInfo(contactManifold);
			contactManifold = contactManifold->getNext();
			colliderNum++;
		}
	}
	void Make_CollisionInfo(reactphysics3d::ContactManifold * _contactManifold) {
		mCollider->mCollisionInfo.push_back(std::make_unique<CollisionInfo>());
		CollisionInfo * collision = mCollider->mCollisionInfo[mCollider->mCollisionInfo.size() - 1].get();
		auto iter = mBody_Collider_MAP->find(_contactManifold->getBody2());
		if (iter != mBody_Collider_MAP->end()) {
			collision->mCollider = iter->second;
			collision->mGameObject = collision->mCollider->gameObject;
		}

		reactphysics3d::Vector3 contactposition;
		DirectX::XMFLOAT3 DcontactPos;
		reactphysics3d::ContactPoint * point = _contactManifold->getContactPoints();
		reactphysics3d::Vector3 centerpos = _contactManifold->getBody1()->getTransform().getPosition();
		DirectX::XMVECTOR centerVec = DirectX::XMVectorSet(centerpos.x, centerpos.y, centerpos.z, 1.0f);

		while (point != nullptr) {
			contactposition = point->getLocalPointOnShape1();
			DirectX::XMVECTOR contactVec = DirectX::XMVectorSet(contactposition.x, contactposition.y, contactposition.z, 0.0f);

			contactposition += centerpos;
			DcontactPos = DirectX::XMFLOAT3(contactposition.x, contactposition.y, contactposition.z);
			
			collision->mContactPoints.push_back(DcontactPos);
			collision->mCenterContactVec.push_back(DirectX::XMVector3Normalize(contactVec));
			point = point->getNext();
		}
	}
	void notifyContact(const CollisionCallbackInfo &collisionCallbackInfo) {
		reactphysics3d::ContactManifoldListElement * maniFoldList = collisionCallbackInfo.contactManifoldElements;
		while (maniFoldList != nullptr) {
			ContactManifoldTraverse(maniFoldList);
			maniFoldList = maniFoldList->getNext();
		}
	}

	Collider_v2 * mCollider = nullptr;
	std::map< reactphysics3d::CollisionBody *, Collider_v2*> *mBody_Collider_MAP;
	std::vector<std::unique_ptr<CollisionInfo>> * vec = nullptr;
	int colliderNum = 0;
};

PhysicsManager::~PhysicsManager()
{
	delete mReactPhysics_DYNAMIC_WORLD;
	delete mGravity;
}

bool PhysicsManager::Initialize()
{
	mGravity = new reactphysics3d::Vector3(0.0, -9.81, 0.0);
	mReactPhysics_DYNAMIC_WORLD = new reactphysics3d::DynamicsWorld(*mGravity);
	mReactPhysics_DYNAMIC_WORLD->setNbIterationsVelocitySolver(15);
	mReactPhysics_DYNAMIC_WORLD->setNbIterationsPositionSolver(8);

	for (std::vector<std::shared_ptr<Collider_v2>>::iterator it = collider_v2Buffer->begin(); it != collider_v2Buffer->end(); it++) {
		reactphysics3d::CollisionBody* collisionBody  = (*it)->initialize_React3D(mReactPhysics_DYNAMIC_WORLD);
		mBody_Collider_MAP.insert(std::make_pair(collisionBody, (*it).get()));
	}

	return true;
}

void PhysicsManager::PhysicsCompoInit(std::shared_ptr<Collider_v2> _component)
{
	reactphysics3d::CollisionBody* collisionBody = _component->initialize_React3D(mReactPhysics_DYNAMIC_WORLD);
	mBody_Collider_MAP.insert(std::make_pair(collisionBody, _component.get()));
}

void PhysicsManager::Update()
{
	UpdateComponent();
}

void PhysicsManager::PreUpdate()
{
	for (std::vector<std::shared_ptr<Collider_v2>>::iterator it = collider_v2Buffer->begin(); it != collider_v2Buffer->end(); it++) {
		if ((*it) == nullptr) assert("collider buffer have nullptr" && 1 == 0);

		bool gameObjectValid = (*it)->gameObject->enabled;
		bool componentValid = (*it)->enabled;
		if (gameObjectValid && componentValid == false) continue;

		(*it)->BeforePhysicsUpdate();
	}
}

void PhysicsManager::PhysicsUpdate()
{
	PreUpdate();
	mReactPhysics_DYNAMIC_WORLD->update(0.02);
	UpdateComponent();
	CollisionTest_ver2();

	//UpdateCollider();
	//CollisionTest();
	//react3DCollisioinTest();
}

void PhysicsManager::UpdateCollider()
{
	for (std::vector<std::shared_ptr<Collider>>::iterator it = colliderBuffer->begin(); it != colliderBuffer->end(); it++) {
		if ((*it) == nullptr) assert("collider buffer have nullptr" && 1 == 0);

		bool gameObjectValid = (*it)->gameObject->enabled;
		bool componentValid = (*it)->enabled;
		if (gameObjectValid && componentValid == false) continue;

		(*it)->Update();
	}
}

void PhysicsManager::UpdateComponent()
{
	for (std::vector<std::shared_ptr<Collider_v2>>::iterator it = collider_v2Buffer->begin(); it != collider_v2Buffer->end(); it++) {
		if ((*it) == nullptr) assert("collider buffer have nullptr" && 1 == 0);

		bool gameObjectValid = (*it)->gameObject->enabled;
		bool componentValid = (*it)->enabled;
		if (gameObjectValid && componentValid == false) continue;

		(*it)->Update();
	}
}

void PhysicsManager::CollisionTest()
{
	typedef std::vector<std::shared_ptr<Collider>>::iterator ITERATOR;

	ITERATOR begin = colliderBuffer->begin();
	ITERATOR end = colliderBuffer->end();
	int test_Range = 1;

	//콜리더 갯수 n 에서 n*(n-1) / 2 번 계산
	for (ITERATOR source = begin + 1; source != end; source++) {
		bool gameObjectValid = (*source)->gameObject->enabled;
		bool componentValid = (*source)->enabled;
		if (gameObjectValid && componentValid == false) continue;

		for (ITERATOR target = begin; target != begin + test_Range; target++) {
			gameObjectValid = (*target)->gameObject->enabled;
			componentValid = (*target)->enabled;
			if (gameObjectValid && componentValid == false) continue;

			//DirectX::ContainmentType result = (*source)->Debug_CollisionTest((*target));
			MyCustom::CollisionType result = (*source)->My_CollisionTest(target->get());

			if (result == 0) continue;

			(*source)->collidObj.push_back(target->get());
			(*target)->collidObj.push_back(source->get());
		}

		test_Range++;
	}
}

void PhysicsManager::CollisionTest_ver2()
{
	typedef std::vector<std::shared_ptr<Collider_v2>>::iterator ITERATOR;

	ITERATOR begin = collider_v2Buffer->begin();
	ITERATOR end = collider_v2Buffer->end();

	//콜리더 갯수 n 에서 n*(n-1) / 2 번 계산
	for (ITERATOR iter = begin + 1; iter != end; iter++) {
		bool gameObjectValid = (*iter)->gameObject->enabled;
		bool componentValid = (*iter)->enabled;
		bool colllisionTestOn = (*iter)->colllisionTestOn;
		if (gameObjectValid && componentValid && colllisionTestOn == false) continue;
		
		react3DCollisioinTest((*iter).get());
	}
}

void PhysicsManager::react3DCollisioinTest()
{
	r3dCallback testCall;

	mReactPhysics_DYNAMIC_WORLD->testCollision(&testCall);
	//mReactPhysics_DYNAMIC_WORLD->testCollision()
}

void PhysicsManager::react3DCollisioinTest(Collider_v2* _collider)
{
	r3dCollisionCallback collisionCallback(_collider, &mBody_Collider_MAP);

	reactphysics3d::CollisionBody * body = _collider->mRigidBody;

	mReactPhysics_DYNAMIC_WORLD->testCollision(body, &collisionCallback);
}
