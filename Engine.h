#pragma once
#include "WindowContainer.h"
#include "Timer.h"
#include "SceneManager.h"
#include "PhysicsManager.h"
#include "AnimationManager.h"

class Light_ver2;
class ScriptBehaviour;

class Engine : WindowContainer{
public:
	Engine()
		: graphicsManager(this, &timer.Time), 
		sceneManager(this, &timer.Time), 
		physicsManager(this, &timer.Time, &physicsBuffer),
		animationManager(this, &timer.Time, &animatorBuffer) {}
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessage();
	void Update();
	void RenderFrame();

	void InsertScriptComponent(Component * component);
	void InsertLightComponent(Light_ver2 * component);
	void InsertTerrainComponent(Terrain * component);
	void InsertColliderComponent(Collider * component);
	void InsertAnimatorComponent(Animator * component);

	std::vector<AnimationClip> * GetAnimClipBuffer();

	~Engine();

private:
	Timer timer;
	Graphics graphicsManager;
	SceneManager sceneManager;
	PhysicsManager physicsManager;
	AnimationManager animationManager;

	std::vector<Component*> scriptBuffer;
	std::vector<Light_ver2*> lightBuffer;
	std::vector<Collider*> physicsBuffer;
	std::vector<Terrain*> terrainBuffer;
	std::vector<Animator*> animatorBuffer;

	float fixedTimeCheck = -1.0f;
	bool ESC_signal = false;
};