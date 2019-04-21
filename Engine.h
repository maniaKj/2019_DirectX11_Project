#pragma once
#include "WindowContainer.h"
#include "Timer.h"
#include "SceneManager.h"

class Light_ver2;
class ScriptBehaviour;

class Engine : WindowContainer{
public:
	Engine() : sceneManager(this, &timer.Time) {}
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessage();
	void Update();
	void RenderFrame();
	void InsertScriptComponent(Component * component);
	void InsertLightComponent(Light_ver2 * component);
	~Engine();

private:
	Timer timer;
	SceneManager sceneManager;

	std::vector<Component*> scriptBuffer;
	std::vector<Light_ver2*> lightBuffer;
	std::vector<Component*> physicsBuffer;
};