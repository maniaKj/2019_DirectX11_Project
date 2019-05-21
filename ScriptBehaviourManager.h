#pragma once

#include <vector>
#include <memory>

class Engine; //include 정의 조심
class GameObject_v2;
class Component;
class ScriptBehaviour;
struct TimeInfo;
class KeyboardClass;
class MouseClass;

class ScriptBehaviourManager {
	friend class Engine;
public:
	void Script_INIT(ScriptBehaviour * _script);

private:
	ScriptBehaviourManager(
		Engine * const _engine_ptr, 
		TimeInfo * const _timeInfo, 
		std::vector<std::shared_ptr<ScriptBehaviour>> * const _buffer, 
		KeyboardClass * const _keyboard, 
		MouseClass * const _mouse) 
		:
		engine(_engine_ptr),
		Time(_timeInfo),
		mScriptBuffer(_buffer),
		keyboard(_keyboard),
		mouse(_mouse) {}

	void Update();
	void Start();

	std::vector<std::shared_ptr<ScriptBehaviour>> * const mScriptBuffer;
	Engine * const engine;
	TimeInfo * const Time;
	KeyboardClass * const keyboard;
	MouseClass * const mouse;
};