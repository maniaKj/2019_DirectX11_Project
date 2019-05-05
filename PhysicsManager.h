#pragma once
#include <vector>
#include <memory>

class Engine; //include 정의 조심
class GameObject_v2;
class Component;
class Collider;
struct TimeInfo;

class PhysicsManager {
	friend class Engine;
private:
	PhysicsManager(Engine * const engine_ptr, TimeInfo * const timeInfo, std::vector<std::shared_ptr<Collider>> * buffer) :
		engine(engine_ptr), 
		Time(timeInfo), 
		colliderBuffer(buffer) {}

	void Update();
	void UpdateCollider();
	void CollisionTest();

	std::vector<std::shared_ptr<Collider>> * const colliderBuffer;
	Engine * const engine;
	TimeInfo * const Time;
};