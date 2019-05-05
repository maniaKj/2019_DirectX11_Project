#include "PhysicsManager.h"
#include "Component/GameObject_v2.h"
#include <cassert>

void PhysicsManager::Update()
{
	UpdateCollider();

	CollisionTest();
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