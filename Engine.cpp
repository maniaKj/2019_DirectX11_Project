#include "Engine.h"
#include "Component/ScriptBehaviour.h"
#include <DirectXMath.h>

using DirectX::operator+=;
using DirectX::operator*;

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) 
{
	//윈도우 초기화
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	//그래픽스 초기화
	if (!graphicsManager.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	graphicsManager.InitializeSimpleGeometry(sceneManager.modelBuffer);
	graphicsManager.InitializeModel(sceneManager.modelBuffer);
	


	//씬 매니저 초기화
	sceneManager.Custom_Test_Obj_Set(); // 테스트용

	graphicsManager.gameObjBuffer = &sceneManager.gameObjectBuffer;
	graphicsManager.lightBuffer = &lightBuffer;
	graphicsManager.terrainBuffer = &terrainBuffer;
	graphicsManager.InitializeTerrain(sceneManager.terrainBuffer);
	
	

	//그래픽스 씬 초기화
	if (!graphicsManager.InitializeScene()) {
		MessageBoxA(NULL, "Initialize Scene Error", "Error", MB_ICONERROR);
		return false;
	}

	//graphicsManager.TestI();

	//스크립트 Start() 실행
	typedef std::vector<std::shared_ptr<ScriptBehaviour>>::iterator ScriptIterator;
	typedef std::shared_ptr<ScriptBehaviour> Script_ptr;
	ScriptIterator begin = scriptBuffer.begin();
	ScriptIterator end = scriptBuffer.end();
	for (ScriptIterator iter = begin; iter != end;) {
		if ((*iter).use_count() <= 1) {
			iter = scriptBuffer.erase(iter);
			end = scriptBuffer.end();
			continue;
		}

		Script_ptr script = (*iter);
		
		bool Component_valid = script.get()->enabled;
		bool GameObject_valid = script.get()->gameObject->enabled;
		if (Component_valid && GameObject_valid) {
			script.get()->Start();
		}
		iter++;
	}

	timer.Start();
	return true;
}

bool Engine::ProcessMessage() {
	return ESC_signal ? false : this->render_window.ProcessMessage();
}

void Engine::Update() {
	timer.Tick();
	float dt = timer.Time.GetDeltaTime();
	fixedTimeCheck -= dt;

	//fixedupdate
	if (fixedTimeCheck < 0.0f) { 
		physicsManager.Update();
		fixedTimeCheck = (float)0.07;
		
	}

	animationManager.Update();

	//게임오브젝트의 스크립트 컴포넌트에 대하여
	//Update() 실행
	typedef std::vector<std::shared_ptr<ScriptBehaviour>>::iterator ScriptIterator;
	typedef std::shared_ptr<ScriptBehaviour> Script_ptr;
	ScriptIterator begin = scriptBuffer.begin();
	ScriptIterator end = scriptBuffer.end();
	for (ScriptIterator iter = begin; iter != end;) {
		if ((*iter).use_count() <= 1) {
			iter = scriptBuffer.erase(iter);
			end = scriptBuffer.end();
			continue;
		}

		Script_ptr script = (*iter);

		bool Component_valid = script.get()->enabled;
		bool GameObject_valid = script.get()->gameObject->enabled;
		if (Component_valid && GameObject_valid) {
			script.get()->Update();
		}
		iter++;
	}

	

#pragma region Input Event
	//키보드, 마우스 입력값 정보
	while (!keyboard.CharBufferIsEmpty()) {
		unsigned char ch = keyboard.ReadChar();
	}
	while (!keyboard.KeyBufferIsEmpty()) {
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}
	while (!mouse.EventBufferIsEmpty()) {
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown()) {//마우스 오른쪽 버튼 눌렀을 때 회전
			if (me.GetType() == MouseEvent::EventType::Raw_MOVE) {
				this->graphicsManager.Camera3D.AdjustRotation((float)me.GetPosY() * 1.0f * dt, (float)me.GetPosX() * 1.0f * dt, 0.0f);
			}
		}
	}
	//1인칭 형식 카메라 이동 조작
	float Camera3DSpeed = 6.0f;

	if (keyboard.KeyIsPressed(VK_SHIFT)) {
		Camera3DSpeed = 30.0f;
	}

	if (keyboard.KeyIsPressed('W')) {
		this->graphicsManager.Camera3D.AdjustPosition(this->graphicsManager.Camera3D.GetForwardVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed('S')) {
		this->graphicsManager.Camera3D.AdjustPosition(this->graphicsManager.Camera3D.GetBackwardVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed('A')) {
		this->graphicsManager.Camera3D.AdjustPosition(this->graphicsManager.Camera3D.GetLeftVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed('D')) {
		this->graphicsManager.Camera3D.AdjustPosition(this->graphicsManager.Camera3D.GetRightVector() * Camera3DSpeed * dt);
	}
	if (keyboard.KeyIsPressed(VK_SPACE)) {
		this->graphicsManager.Camera3D.AdjustPosition(0.0f, Camera3DSpeed * dt, 0.0f);
	}
	if (keyboard.KeyIsPressed('Z')) {
		this->graphicsManager.Camera3D.AdjustPosition(0.0f, -Camera3DSpeed * dt, 0.0f);
	}

	if (keyboard.KeyIsPressed('C')) {
		DirectX::XMVECTOR lightPosition = this->graphicsManager.Camera3D.GetPositionVector();
		lightPosition += this->graphicsManager.Camera3D.GetForwardVector();
		this->graphicsManager.light.SetPosition(lightPosition);
		this->graphicsManager.light.SetRotation(this->graphicsManager.Camera3D.GetRotationFloat3());
	}
	static int ttttt = 0;
	if (keyboard.KeyIsPressed('P') && ttttt == 0) {
		/*delete scriptBuffer[1];
		ttttt = 1;
		sceneManager.gameObjectBuffer[0]->componentBuffer.erase(sceneManager.gameObjectBuffer[0]->componentBuffer.begin() + 1);
		scriptBuffer.erase(scriptBuffer.begin() + 1);*/
		ESC_signal = true;
	}
#pragma endregion

}

void Engine::RenderFrame()
{
	
	graphicsManager.RenderFrame();
	//graphicsManager.DebugDrawTest();
	graphicsManager.RenderColliderDebug(&physicsBuffer);
	graphicsManager.SwapBuffer();
}

void Engine::InsertScriptComponent(ScriptBehaviour * _component, GameObject_v2 *_destination)
{
	assert("component is null ptr & add component." && _component != nullptr);
	scriptBuffer.push_back(std::shared_ptr<ScriptBehaviour>(_component));
	_destination->componentBuffer.push_back(std::shared_ptr<Component>(scriptBuffer[scriptBuffer.size() - 1]));
}

void Engine::InsertLightComponent(Light_ver2 * _component, GameObject_v2 *_destination)
{
	assert("component is null ptr & add component." && _component != nullptr);
	lightBuffer.push_back(std::shared_ptr<Light_ver2>(_component));
	_destination->componentBuffer.push_back(std::shared_ptr<Component>(lightBuffer[lightBuffer.size() - 1]));
}

void Engine::InsertTerrainComponent(Terrain * _component, GameObject_v2 *_destination)
{
	assert("component is null ptr & add component." && _component != nullptr);
	terrainBuffer.push_back(std::shared_ptr<Terrain>(_component));
	_destination->componentBuffer.push_back(std::shared_ptr<Component>(terrainBuffer[terrainBuffer.size() - 1]));
}

void Engine::InsertColliderComponent(Collider * _component, GameObject_v2 *_destination)
{
	assert("component is null ptr & add component." && _component != nullptr);
	physicsBuffer.push_back(std::shared_ptr<Collider>(_component));
	_destination->componentBuffer.push_back(std::shared_ptr<Component>(physicsBuffer[physicsBuffer.size() - 1]));
}

void Engine::InsertAnimatorComponent(Animator * _component, GameObject_v2 *_destination) 
{
	assert("component is null ptr & add component." && _component != nullptr);
	//animatorBuffer.push_back(std::shared_ptr<Animator>(_component));
	animatorBuffer.push_back(std::shared_ptr<Animator>(_component));
	_destination->componentBuffer.push_back(std::shared_ptr<Component>(animatorBuffer[animatorBuffer.size() - 1]));
}

void Engine::Component_Valid_Test()
{
	for (std::vector<std::shared_ptr<ScriptBehaviour>>::iterator it = scriptBuffer.begin(); it != scriptBuffer.end();) {
		if (it->unique()) {
			it = scriptBuffer.erase(it);
			continue;
		}
		it++;
	}
	for (std::vector<std::shared_ptr<Light_ver2>>::iterator it = lightBuffer.begin(); it != lightBuffer.end();) {
		if (it->unique()) {
			it = lightBuffer.erase(it);
			continue;
		}
		it++;
	}
	for (std::vector<std::shared_ptr<Collider>>::iterator it = physicsBuffer.begin(); it != physicsBuffer.end();) {
		if (it->unique()) {
			it = physicsBuffer.erase(it);
			continue;
		}
		it++;
	}
	for (std::vector<std::shared_ptr<Terrain>>::iterator it = terrainBuffer.begin(); it != terrainBuffer.end();) {
		if (it->unique()) {
			it = terrainBuffer.erase(it);
			continue;
		}
		it++;
	}
	for (std::vector<std::shared_ptr<Animator>>::iterator it = animatorBuffer.begin(); it != animatorBuffer.end();) {
		if (it->unique()) {
			it = animatorBuffer.erase(it);
			continue;
		}
		it++;
	}
}

std::vector<AnimationClip>* Engine::GetAnimClipBuffer()
{
	return &animationManager.mAnimClipBuffer;
}

Engine::~Engine()
{
}
