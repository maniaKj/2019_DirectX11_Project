#include "Engine.h"
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
	for (std::vector<Component *>::iterator it = scriptBuffer.begin(); it != scriptBuffer.end(); it++) {
		bool Component_valid = (*it)->enabled;
		bool GameObject_valid = (*it)->gameObject->enabled;
		if (Component_valid && GameObject_valid) {
			(*it)->Start();
		}
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
		fixedTimeCheck = 0.07;
		
	}

	animationManager.Update();

	//게임오브젝트의 스크립트 컴포넌트에 대하여
	//Update() 실행
	for (std::vector<Component*>::iterator it = scriptBuffer.begin(); it != scriptBuffer.end(); it++) {
		bool Component_valid = (*it)->enabled;
		bool GameObject_valid = (*it)->gameObject->enabled;
		if (Component_valid && GameObject_valid) {
			(*it)->Update();
		}
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

void Engine::InsertScriptComponent(Component * component)
{
	assert("component is null ptr & add component." && component != nullptr);
	scriptBuffer.push_back(component);
}

void Engine::InsertLightComponent(Light_ver2 * component)
{
	assert("component is null ptr & add component." && component != nullptr);
	lightBuffer.push_back(component);
}

void Engine::InsertTerrainComponent(Terrain * component)
{
	assert("component is null ptr & add component." && component != nullptr);
	terrainBuffer.push_back(component);
}

void Engine::InsertColliderComponent(Collider * component)
{
	assert("component is null ptr & add component." && component != nullptr);
	physicsBuffer.push_back(component);
}

void Engine::InsertAnimatorComponent(Animator * component) {
	assert("component is null ptr & add component." && component != nullptr);
	animatorBuffer.push_back(component);
}

std::vector<AnimationClip>* Engine::GetAnimClipBuffer()
{
	return &animationManager.mAnimClipBuffer;
}

Engine::~Engine()
{
	
}
