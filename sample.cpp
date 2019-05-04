#include "ErrorLogger.h"
#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_  HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	/*HRESULT hr = S_OK;
	if (SUCCEEDED(hr)) {
		MessageBoxA(NULL, "SUCCESS", "SUCCESS", NULL);
	}

	if (FAILED(hr)) {
		ErrorLogger::Log(E_INVALIDARG, "Test Message");
	}*/

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		ErrorLogger::Log(hr, "Failed to call CoInitialize.");
		return -1;
	}

	//COMException exception(0, "TEST MESSAGE", __FILE__, __FUNCTION__, __LINE__);
	//ErrorLogger::Log(exception);

	

	Engine engine;
	engine.Initialize(hInstance, "DirectX Study", "class", 1280, 768);
	while (engine.ProcessMessage() == true) {
		engine.Update();
		engine.RenderFrame();
	}
	engine.~Engine();

	//system("pause");
	return 0;
}