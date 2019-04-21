#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) 
{
	//윈도우 초기화
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	//그래픽스 초기화
	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	gfx.InitializeModel(sceneManager.modelBuffer);

	//씬 매니저 초기화
	sceneManager.Custom_Test_Obj_Set(); // 테스트용
	gfx.gameObjBuffer = &sceneManager.gameObjectBuffer;
	gfx.lightBuffer = &lightBuffer;

	//그래픽스 씬 초기화
	if (!gfx.InitializeScene()) {
		MessageBoxA(NULL, "Initialize Scene Error", "Error", MB_ICONERROR);
		return false;
	}

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
	return this->render_window.ProcessMessage();
}

void Engine::Update() {
	timer.Tick();
	float dt = timer.Time.GetDeltaTime();

	//게임오브젝트의 스크립트 컴포넌트에 대하여
	//Update() 실행
	for (std::vector<Component*>::iterator it = scriptBuffer.begin(); it != scriptBuffer.end(); it++) {
		bool Component_valid = (*it)->enabled;
		bool GameObject_valid = (*it)->gameObject->enabled;
		if (Component_valid && GameObject_valid) {
			(*it)->Update();
		}
	}

	{
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
					this->gfx.Camera3D.AdjustRotation((float)me.GetPosY() * 1.0f * dt, (float)me.GetPosX() * 1.0f * dt, 0.0f);
				}
			}
		}
		//1인칭 형식 카메라 이동 조작
		float Camera3DSpeed = 6.0f;

		if (keyboard.KeyIsPressed(VK_SHIFT)) {
			Camera3DSpeed = 30.0f;
		}

		if (keyboard.KeyIsPressed('W')) {
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetForwardVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed('S')) {
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetBackwardVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed('A')) {
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetLeftVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed('D')) {
			this->gfx.Camera3D.AdjustPosition(this->gfx.Camera3D.GetRightVector() * Camera3DSpeed * dt);
		}
		if (keyboard.KeyIsPressed(VK_SPACE)) {
			this->gfx.Camera3D.AdjustPosition(0.0f, Camera3DSpeed * dt, 0.0f);
		}
		if (keyboard.KeyIsPressed('Z')) {
			this->gfx.Camera3D.AdjustPosition(0.0f, -Camera3DSpeed * dt, 0.0f);
		}

		if (keyboard.KeyIsPressed('C')) {
			XMVECTOR lightPosition = this->gfx.Camera3D.GetPositionVector();
			lightPosition += this->gfx.Camera3D.GetForwardVector();
			this->gfx.light.SetPosition(lightPosition);
			this->gfx.light.SetRotation(this->gfx.Camera3D.GetRotationFloat3());
		}

		if (keyboard.KeyIsPressed('P')) {
			//delete this;
		}
	}
}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
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

Engine::~Engine()
{
	
}
