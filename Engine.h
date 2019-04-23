#pragma once
#include "WindowContainer.h"
#include "Timer.h"
#include "SceneManager.h"
#include "PhysicsManager.h"

class Light_ver2;
class ScriptBehaviour;

class Engine : WindowContainer{
public:
	Engine() : sceneManager(this, &timer.Time), physicsManager(this, &timer.Time, &physicsBuffer) {}
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessage();
	void Update();
	void RenderFrame();

	void InsertScriptComponent(Component * component);
	void InsertLightComponent(Light_ver2 * component);
	void InsertTerrainComponent(Terrain * component);
	void InsertColliderComponent(Collider * component);

	~Engine();

private:
	Timer timer;
	SceneManager sceneManager;
	PhysicsManager physicsManager;

	std::vector<Component*> scriptBuffer;
	std::vector<Light_ver2*> lightBuffer;
	std::vector<Collider*> physicsBuffer;
	std::vector<Terrain*> terrainBuffer;

	float fixedTimeCheck = -1.0f;
};