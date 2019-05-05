#pragma once

#include <vector>
#include <memory>

class Engine; //include 정의 조심
class GameObject_v2;
class Component;
class ScriptBehaviour;
struct TimeInfo;

class ScriptBehaviourManager {
	friend class Engine;
private:
	ScriptBehaviourManager(Engine * const _engine_ptr, TimeInfo * const _timeInfo, std::vector<std::shared_ptr<ScriptBehaviour>> * _buffer) :
		engine(_engine_ptr),
		Time(_timeInfo),
		mScriptBuffer(_buffer) {}

	void Update();
	void Start();

	std::vector<std::shared_ptr<ScriptBehaviour>> * const mScriptBuffer;
	Engine * const engine;
	TimeInfo * const Time;
};