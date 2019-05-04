#pragma once
#include <vector>

class Engine; //include ���� ����
class GameObject_v2;
class Component;
class Collider;
struct TimeInfo;

class PhysicsManager {
	friend class Engine;
private:
	PhysicsManager(Engine * const engine_ptr, TimeInfo * const timeInfo, std::vector<Collider*> * buffer) : 
		engine(engine_ptr), 
		Time(timeInfo), 
		colliderBuffer(buffer) {}

	void Update();
	void UpdateCollider();
	void TestCollider();

	std::vector<Collider*> * const colliderBuffer;
	Engine * const engine;
	TimeInfo * const Time;
};