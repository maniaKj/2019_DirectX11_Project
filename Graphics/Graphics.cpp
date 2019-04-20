#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height) {
	this->windowWidth = width;
	this->windowHeight = height;
	this->fpsTimer.Start();


	if (!InitializeDirectX(hwnd)) {
		MessageBoxA(NULL, "Initialize DX Error", "Error", MB_ICONERROR);
		return false;
	}


	if (!InitializeShaders()) {
		MessageBoxA(NULL, "Initialize Shader Error", "Error", MB_ICONERROR);
		return false;
	}

	/*if (!InitializeScene()) {
		MessageBoxA(NULL, "Initialize Scene Error", "Error", MB_ICONERROR);
		return false;
	}*/
		

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();

	return true;
}

void Graphics::RenderFrame()
{
	//상수 버퍼 셋팅
	cb_ps_light.data.dynamicLightColor = light.lightColor;
	cb_ps_light.data.dynamicLightStrength = light.lightStrength;
	cb_ps_light.data.dynamicLightPosition = light.GetPositionFloat3();
	cb_ps_light.data.dynamicLightAttenuation_a = light.attenuation_a;
	cb_ps_light.data.dynamicLightAttenuation_b = light.attenuation_b;
	cb_ps_light.data.dynamicLightAttenuation_c = light.attenuation_c;
	cb_ps_light.ApplyChanges();
	deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_light.GetAddressOf());

	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor); //화면 깨끗하게 만들기
	deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //도형 그리기 위상구조 설정. ->삼각형으로 그리겠다.
	deviceContext->RSSetState(this->rasterizerState.Get());//래스터라이저 설정 적용
	deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);

	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->OMSetBlendState(blendState.Get(), blendFactors, 0xFFFFFFFF);
	/* OM 단계에 블렌딩 스테이트 적용
	virtual void STDMETHODCALLTYPE OMSetBlendState(
		_In_opt_  ID3D11BlendState *pBlendState,	//장치에 적용할 혼합 상태 객체
		_In_opt_  const FLOAT BlendFactor[4],		//부동소수점 값 네 개의 배열을 가리키는 포인터, RGBA색상 벡터 정의
		_In_  UINT SampleMask) = 0;					//다중표본화로 취하는 최대 32개의 표본. 지금은 0xFFFFFFFF 사용
	*/
	deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

	
	
	{
		//모든 오브젝트 모델 렌더링
		const DirectX::XMMATRIX viewProjectionMatrix = Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix();

		for (std::vector<GameObject_v2*>::iterator it = all_gameObj->begin(); it != all_gameObj->end(); it++) {
			deviceContext->IASetInputLayout(this->All_vertexShader[(*it)->renderer.vertexshaderID]->GetInputLayout()); //IA에 입력할 배치 적용
			deviceContext->VSSetShader(this->All_vertexShader[(*it)->renderer.vertexshaderID]->GetShader(), NULL, 0); //그릴 때 쓸 셰이더 적용
			deviceContext->PSSetShader(this->All_pixelShader[(*it)->renderer.pixelshaderID]->GetShader(), NULL, 0); //그릴 때 쓸 셰이더 적용
			(*it)->renderer.Renderer_Draw(viewProjectionMatrix);
		}
	}
	{
		deviceContext->PSSetShader(pixelshader_nolight.GetShader(), NULL, 0);
		light.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
	}
	
	//Draw text + 프레임 측정
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	float tmp = fpsTimer.GetMilisecondsElapsed();
	float dt = fpsTimer.Time.deltaTime;
	fpsTimer.Tick();
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0) {
		fpsString = "FPS: " + std::to_string(fpsCounter) + ", deltaTime : " + std::to_string(fpsTimer.Time.deltaTime);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringHelper::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	static int counter = 0;
	//Start the Dear ImGui Frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//Create ImGui Test Window
	ImGui::Begin("Light Controls");
	ImGui::DragFloat3("Ambient Light Color", &this->cb_ps_light.data.ambientLightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Light Strenght", &this->cb_ps_light.data.ambientLightStrength, 0.01f, 0.0f, 1.0f);
	ImGui::NewLine();
	ImGui::DragFloat3("Dynamic Light Color", &this->light.lightColor.x, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Strength", &this->light.lightStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation A", &this->light.attenuation_a, 0.01f, 0.1f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation B", &this->light.attenuation_b, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Dynamic Light Attenuation C", &this->light.attenuation_c, 0.01f, 0.0f, 10.0f);
	ImGui::End();

	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	this->swapchain->Present(1, NULL); //vsync 설정, 1이면 프레임 동기화, 0이면 프레임 최대
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	try {
		HRESULT hr;

		

		//어댑터 불러오기
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}

		//******************************
		//******스왑 체인 구조체 설정
		DXGI_SWAP_CHAIN_DESC scd = { 0 };

		scd.BufferDesc.Width = this->windowWidth; //창 높이
		scd.BufferDesc.Height = this->windowHeight; //창 너비
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		//SampleDesc 다중 표본화Multi sampling 를 위해 추출할 표본 개수와 품질 수준 
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼 용도 -> DXGI_USAGE_RENDER_TARGET_OUTPUT 후면 버퍼에 렌더링
		scd.BufferCount = 1; //후면 버퍼 갯수 -> 1은 후면 버퍼 한개(이중 버퍼링)
		scd.OutputWindow = hwnd; //렌더링 결과를 받을 핸들
		scd.Windowed = TRUE; // 창모드설정
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //교환 효과 서술 -> DXGI_SWAP_EFFECT_DISCARD 디스플레이 구동기가 가장 효과적인 제시 방법
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; 
		//추가 플래그 -> DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH 
		//응용 프로그램이 전체화면 모드로 전환할 때 현재의 후면 버퍼 설정에 가장 잘 부합하는 디스플레이 모드가 자동으로 선택

		
		//디버그 플래그 설정
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		
		//******************************
		hr = D3D11CreateDeviceAndSwapChain(
			adapters[0].pAdapter, //IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, //FOR SOFTWARE DRIVER TYPE
			createDeviceFlags, //FLAGS FOR RUNTIME LAYERS
			NULL, //FEATURE LEVELS ARRAY
			0, //# OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION, //사용하는 버전, 무조건 D3D11_SDK_VERSION 사용
			&scd, //Swapchain description
			this->swapchain.GetAddressOf(), //Swapchain Address
			this->device.GetAddressOf(), //Device Address
			NULL, //Supported feature level
			this->deviceContext.GetAddressOf()); //Device Context Address

		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		//MSAA Multisample anti-aliasing 설정
		UINT m4xMsaaQuality;
		hr = this->device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
		//주어진 텍스쳐 형식의 표본개수의 조합에 대한 품질 수준들의 개수
		COM_ERROR_IF_FAILED(hr, "Failed to set Msaa QualityLevel.");
		/*다중표본화 관련 구조체
		typedef struct DXGI_SAMPLE_DESC
		{
			UINT Count;		//픽셀당 추출할 표본 개수 지정
			UINT Quality;	//원하는 품질 수준
		} DXGI_SAMPLE_DESC;
		*/

		//******************************
		//******Render target view 생성
		//자원을 파이프라인 단계에 직접 묶는 것이 아닌 자원에 대한 뷰를 생성하여 그 뷰를 파이프라인에 묶는다.
		//후면 버퍼를 파이프라인의 출력 병합기output merger 단계에 묶기 -> Direct3D가 버퍼에 뭔가를 그릴 수 있게 함

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer; //후면 버퍼
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		/* GetBuffer 메소드는 swapchain을 가리키는 포인터를 가져온다.
		virtual HRESULT STDMETHODCALLTYPE GetBuffer(
			UINT Buffer,					//얻고자 하는 후면 버퍼의 색인(후면 버퍼가 여러 개 있는 경우 중요. 지금은 하나니까 0)
			_In_  REFIID riid,				//버퍼의 인터페이스 형식 지정. 일반적으로는 항상 2차원 텍스쳐를 위한 ID3D11Texture2D가 쓰임
			_COM_Outptr_  void **ppSurface) = 0;	//후면 버퍼를 가리키는 포인터를 돌려줌
		*/

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		/* 렌더 뷰를 생성하는 메소드
		virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
			_In_  ID3D11Resource *pResource,							//렌더 대상으로 사용할 자원. 후면 버퍼 지정.
			_In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,		//자원에 담긴 원소들의 자료 형식 서술.
			_COM_Outptr_opt_  ID3D11RenderTargetView **ppRTView) = 0;	//생성된 렌더 대상 뷰를 되돌려줌
		*/

		//******************************
		//******Describe our Depth/Stencil Buffer
		//깊이 버퍼는 그냥 깊이 정보를(스텐실도 마찬가지) 담는 2차원 텍스쳐.
		//2차원 텍스쳐를 생성할 때는 아래 구조체 활용.  + 생성자 활용해서 코드 줄이기(애초에 default 패러미터 값은 안 건드려도 됨)
		CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->windowWidth, this->windowHeight);
		depthStencilDesc.MipLevels = 1; 
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		/*if (mEnable4xMSAA) {
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
		}
		else {
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}*/

		/*
		typedef struct D3D11_TEXTURE2D_DESC
		{
		UINT Width;		//높이 너비
		UINT Height;
		UINT MipLevels; //밉맵 수준의 개수, 깊이/스텐실 버퍼를 위한 텍스쳐에서는 하나면 충분
		UINT ArraySize;	//텍스처 배열의 텍스쳐 개수, 깊이/스텐실 버퍼의 경우 하나면 충분
		DXGI_FORMAT Format;	//텍셀의 형식. 이 경우 DXGI_FORMAT_D24_UNORM_S8_UINT 사용 -> 깊이 24비트 [0, 1]구간, 스텐실 8비트 부호 없는 정수
		DXGI_SAMPLE_DESC SampleDesc;	//다중 표본 개수와 품질 수준을 서술 -> 안티 앨리어싱
		D3D11_USAGE Usage;				//텍스쳐의 용도. 지금은 D3D11_USAGE_DEFAULT -> cpu는 이 자원을 쓸 수 없고 gpu가 사용/작업 수행
		UINT BindFlags;				//자원을 파이프라인에 어떻게 묶을 것인지 지정. 지금은 D3D11_BIND_DEPTH_STENCIL -> 깊이/스텐실 버퍼 전용
		UINT CPUAccessFlags;		//cpu가 자원에 접근하는 방식을 결정하는 플래그. 지금 cpu는 자원에 접근하지 않으므로 0지정.
		UINT MiscFlags;				//기타 플래그. 깊이/스텐실 버퍼에는 적용되지 않으므로 0지정.
		} 	D3D11_TEXTURE2D_DESC;
		*/

		hr = this->device->CreateTexture2D(
			&depthStencilDesc,							//생성할 텍스쳐를 서술하는 구조체
			NULL,										//텍스쳐에 채울 초기 자료. 깊이/스텐실 버퍼에서는 따로 필요없음
			this->depthStencilBuffer.GetAddressOf());	//깊이 스텐실 버퍼를 가리키는 포인터를 돌려준다.
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(
			this->depthStencilBuffer.Get(),			//뷰를 생성하고자 하는 자원
			NULL,									//D3D11_DEPTH_STENCIL_VIEW_DESC 구조체를 가리키는 포인터. 자원 자료형 서술. 따료 필요 없음
			this->depthStencilView.GetAddressOf()); //깊이 스텐실 뷰를 돌려준다.
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		//******************************
		//후면 버퍼와 깊이스텐실 버퍼를 생성했으니, 뷰들을 파이프라인 출력 병합기에 묶기
		//이 과정을 거쳐야 비로소 자원들이 파이프라인의 렌더 대상과 깊이.스텐실 버퍼로 작용
		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());
		//첫번째 파라미터는 묶고자 하는 렌더 대상의 개수 ->여기에서는 하나.
		//두번째 세번째는, 뷰 자원들.

		//Create depth stencil state 스텐실 & 뎁스
		CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		//스텐실 draw mask
		CD3D11_DEPTH_STENCIL_DESC depthstencildesc_drawMask(D3D11_DEFAULT);
		depthstencildesc_drawMask.DepthEnable = FALSE;
		depthstencildesc_drawMask.StencilEnable = TRUE;

		depthstencildesc_drawMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		depthstencildesc_drawMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_drawMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_drawMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		depthstencildesc_drawMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
		depthstencildesc_drawMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_drawMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_drawMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;

		hr = this->device->CreateDepthStencilState(&depthstencildesc_drawMask, this->depthStencilState_drawMask.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state for Drawing Mask.");

		//스텐실 apply mask
		CD3D11_DEPTH_STENCIL_DESC depthstencildesc_applyMask(D3D11_DEFAULT);
		depthstencildesc_applyMask.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		depthstencildesc_applyMask.StencilEnable = TRUE;

		depthstencildesc_applyMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		depthstencildesc_applyMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_applyMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_applyMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

		depthstencildesc_applyMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		depthstencildesc_applyMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_applyMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
		depthstencildesc_applyMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;

		//hr = this->device->CreateDepthStencilState(&depthstencildesc_applyMask, this->depthStencilState_applyMask.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state for applying Mask.");

		//**********************
		//뷰포트 만들기 & 세팅
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));
		this->deviceContext->RSSetViewports(1, &viewport); //패러미터 : 적용할 묶을 뷰포트 개수, 적용할 뷰포트 포인터,
		/* 뷰포트 구조체
		typedef struct D3D11_VIEWPORT
		{
		FLOAT TopLeftX; //창의 위치와 크기, 기준은 뷰포트를 표시할 응용 프로그램 창의 클라이언트 영역 직사각형
		FLOAT TopLeftY;
		FLOAT Width;
		FLOAT Height;
		FLOAT MinDepth; //최소 최대 깊이 버퍼 값. 웬만하면 기본값으로 설정할 것.
		FLOAT MaxDepth;
		} 	D3D11_VIEWPORT;
		*/

		//**********************
		//래스터라이저 state 설정
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		//rasterizerDesc.CullMode = D3D11_CULL_NONE;
		hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf()); 
		//설정 구조체를 적용해서 결과 래스터라이저 스테이트를 돌려받음. 나중에 필요한 스테이트를 RSSetState 함수로 적용하면 된다.
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");
		/*래스터라이저 desc
		typedef struct D3D11_RASTERIZER_DESC
		{
		D3D11_FILL_MODE FillMode;	//와이어 프레임 렌더링인 경우 D3D11_FILL_WIREFRAME, solid렌더링인 경우 D3D11_FILL_SOLID
		D3D11_CULL_MODE CullMode;	//선별을 비활성화하려면 D3D11_CULL_NONE, 후면 삼각형을 선별해서 제외시키려면 D3D11_CULL_BACK(FRONT는 전면) 
		BOOL FrontCounterClockwise;	//true일 때 카메라 기준 반시계방향으로 감긴 삼각형을 전면으로 간주. 시계방향이면 false
		INT DepthBias;
		FLOAT DepthBiasClamp;
		FLOAT SlopeScaledDepthBias;
		BOOL DepthClipEnable;
		BOOL ScissorEnable;
		BOOL MultisampleEnable;
		BOOL AntialiasedLineEnable;
		} 	D3D11_RASTERIZER_DESC;
		*/


		//cull front 래스터라이저 state 설정
		CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//**********************
		//Create Blend state
		D3D11_BLEND_DESC blendDesc = { 0 };
		/*블렌딩 상태 서술 구조체
		typedef struct D3D11_BLEND_DESC
		{
		BOOL AlphaToCoverageEnable;	//기본값 false, true로 설정하면 식물 잎사귀 등을 묘사하기에 유용한 alpha-to-coverage 변환이 활성화
		BOOL IndependentBlendEnable;	//기본값 false, true 일때 각 렌더 대상(윈도우)마다 혼합을 개별적으로 수행.
		D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[ 8 ];	//이 배열의 i번째 원소는 i번째 렌더 대상에 적용할 혼합 설정을 담은 구조체
		} 	D3D11_BLEND_DESC;
		*/

		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
		//blend alpha : Final alpha = source alpha * source alpha factor +(blend Operation) destination alpha * destination alpha factor

		rtbd.BlendEnable = true;									//혼합 활성화 여부
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;			//RGB성분 혼합의 원본 혼합 계수
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;	//RGB성분 혼합의 대상 혼합 계수
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;			//RGB성분 혼합 연산자
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;			//알파 성분 혼합의 원본 혼합 계수
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;		//알파 성분 혼합의 대상 혼합 계수
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;		//알바 성분 혼합 연산자
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;	//혼합 결과를 어떤 색상 채널에 기록할 것인지 결정

		blendDesc.RenderTarget[0] = rtbd;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");
		/* //블렌드인터페이스 생성 메소드
		virtual HRESULT STDMETHODCALLTYPE CreateBlendState( 
			_In_  const D3D11_BLEND_DESC *pBlendStateDesc, //혼합 상태를 서술하는 구조체
			_COM_Outptr_opt_  ID3D11BlendState **ppBlendState) = 0; //생성된 블렌딩 인터페이스를 돌려준다.
		*/

		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		//Create sampler description for sampler state
		CD3D11_SAMPLER_DESC sampDesc(D3D11_DEFAULT);
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf()); //Create sampler state
		COM_ERROR_IF_FAILED(hr, "Failed to create sampler state.");
	}
	catch (COMException & exception) {
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders()
{
	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath

#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
#else //x86(win32)
		shaderfolder = L"..\\Debug\\";
#endif 
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
#else //x86(win32)
		shaderfolder = L"..\\Release\\";
#endif
#endif
	
	Find_Shader_File(shaderfolder);

	//2D shader
	D3D11_INPUT_ELEMENT_DESC layout2D[] = {
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA},
	};

	UINT numElements2D = ARRAYSIZE(layout2D);

	if (!vertexshader_2d.Initialize(this->device, shaderfolder + L"vertexshader_2d.cso", layout2D, numElements2D, "vertexshader_2d"))
		return false;

	if (!pixelshader_2d.Initialize(this->device, shaderfolder + L"pixelshader_2d.cso", "pixelshader_2d"))
		return false;

	if (!pixelshader_nolight.Initialize(this->device, shaderfolder + L"pixelshader_nolight.cso", "pixelshader_nolight"))
		return false;

	if (!pixelshader_2d_discard.Initialize(this->device, shaderfolder + L"pixelshader_2d_discard.cso", "pixelshader_2d_discard"))
		return false;


	return true;
}

bool Graphics::InitializeScene()
{
	try {
		//임시 테스트용
		std::string cat_filepath = "Data\\Objects\\Test_cat\\12221_Cat_v1_l3.obj";
		std::string nanosuit_filepath = "Data\\Objects\\nanosuit\\nanosuit.obj";

		//텍스쳐 불러오기
		HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\test.png", nullptr, grassTexture.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create wic texture from file.");

		//Initialize Constant buffer(s)
		hr = this->cb_vs_vertexshader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to Initialize constant buffer.");

		hr = this->cb_vs_vertexshader_2d.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to Initialize 2d constant buffer.");

		hr = this->cb_ps_light.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to Initialize constant buffer.");

		this->cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		this->cb_ps_light.data.ambientLightStrength = 1.0f;

		//광원 데이터 초기화
		if (!light.Initialize(this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader)) {
			MessageBoxA(NULL, "Initialize light Error", "Error", MB_ICONERROR);
			return false;
		}

		//스프라이트 생성
		//if (!sprite.Initialize(this->device.Get(), this->deviceContext.Get(), 256, 256, "Data/Textures/circle.png", cb_vs_vertexshader_2d))
		//{
		//	MessageBoxA(NULL, "Initialize sprite Error", "Error", MB_ICONERROR);
		//	return false;
		//}
		//sprite.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		sprite.SetPosition(XMFLOAT3(windowWidth / 2 - sprite.GetWidth() / 2, windowHeight / 2 - sprite.GetHeight() / 2, 0.0f));

		Camera2D.SetProjectionValues((float)windowWidth, (float)windowHeight, 0.0f, 1.0f);

		Camera3D.SetPosition(0.0f, 0.0f, -2.0f);
		Camera3D.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);

		/*for (std::vector<GameObject_v2*>::iterator it = all_gameObj->begin(); it != all_gameObj->end(); it++) {
			(*it)->renderer.Renderer_Initialize(this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader);
		}*/
	}
	catch (COMException & exception) {
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

void Graphics::Find_Shader_File(std::wstring & shaderfolder) //나중에 쉐이더 초기화 함수와 합칠 것
{
	//폴더 안에 hlsl파일 확인 후 
	//std::string path = "hlsl\\*.hlsl"; // hlsl 파일 경로. 현재 프로젝트 파일과 같은 폴더 안에 있다고 가정.
	std::string pixelShader_path = "hlsl\\PixelShader\\*.hlsl";
	std::string vertexShader_path = "hlsl\\VertexShader\\*.hlsl";
	std::string shader_filename;
	struct _finddata_t fd;
	intptr_t handle;

	//3D shader
	D3D11_INPUT_ELEMENT_DESC layout3D[] = {
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA},
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA}
		//{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA}
	};
	/* 입력 배치 객체, 정점 정보를 direct3d에게 전달
	typedef struct D3D11_INPUT_ELEMENT_DESC
    {
    LPCSTR SemanticName;				//성분에 부여된 문자열 이름
    UINT SemanticIndex;					//semantic에 부여된 색인, 정점 하나에 텍스쳐 좌표가 여러개일 때 색인으로 구분
    DXGI_FORMAT Format;					//정점 성분의 자료 형식
    UINT InputSlot;						//이 성분의 자료가 공급될 정점 버퍼 슬롯의 색인
    UINT AlignedByteOffset;				//입력 슬롯을 하나만 사용하는 경우. 정점 구조체의 시작 위치와 이 정점 성분의 시작위치의 오프셋(바이트 단위)
    D3D11_INPUT_CLASSIFICATION InputSlotClass;
    UINT InstanceDataStepRate;
    } 	D3D11_INPUT_ELEMENT_DESC;
	*/

	UINT numElements3D = ARRAYSIZE(layout3D);

	//vertex shader 초기화
	if ((handle = _findfirst(vertexShader_path.c_str(), &fd)) != -1L) {
		do {
			shader_filename = StringHelper::EraseFileExtension(fd.name);

			VertexShader *vs = new VertexShader;
			if (!vs->Initialize(this->device, shaderfolder + StringHelper::StringToWide(shader_filename + ".cso"), layout3D, numElements3D, shader_filename))
				MessageBoxA(NULL, "Shader Initialize error.", ERROR, MB_ICONERROR);
			All_vertexShader.push_back(vs);


		} while (_findnext(handle, &fd) == 0);
	}

	//pixel shader 초기화
	if ((handle = _findfirst(pixelShader_path.c_str(), &fd)) != -1L) {
		do {
			shader_filename = StringHelper::EraseFileExtension(fd.name);

			PixelShader *ps = new PixelShader;
			if (!ps->Initialize(this->device, shaderfolder + StringHelper::StringToWide(shader_filename + ".cso"), shader_filename))
				MessageBoxA(NULL, "Shader Initialize error.", ERROR, MB_ICONERROR);
			All_pixelShader.push_back(ps);

		} while (_findnext(handle, &fd) == 0);
	}

	_findclose(handle);
}

void Graphics::InitializeModel(ModelBuffer & modelBuffer)
{
	std::string filePath = "Data\\Objects\\";
	InitializeModel(modelBuffer, filePath);
}

void Graphics::InitializeModel(ModelBuffer & modelBuffer, std::string & filePath)
{
	std::string path = filePath + "*.*";

	//std::string extension_all = "*.*", extension_fbx = "*.fbx", extension_obj = "*.obj";

	static std::string debug_string = "";

	struct _finddata_t fd;
	intptr_t handle;

	if ((handle = _findfirst(path.c_str(), &fd)) != -1L) {
		do {
			if (StringHelper::GetFileExtension(fd.name) == "fbx") {
				debug_string += (std::string)fd.name + ", ";

				Model *model = new Model();
				if (!model->Initialize(filePath + fd.name, this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader)) {
					MessageBoxA(NULL, "Model Initialize error.", ERROR, MB_ICONERROR);
					return;
				}
				modelBuffer.buffer.push_back(model);
			}
			else if (StringHelper::GetFileExtension(fd.name) == "obj") {
				debug_string += (std::string)fd.name + ", ";

				Model *model = new Model();
				if (!model->Initialize(filePath + fd.name, this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader)) {
					MessageBoxA(NULL, "Model Initialize error.", ERROR, MB_ICONERROR);
					return;
				}
				modelBuffer.buffer.push_back(model);
			}
			else if (fd.attrib & _A_SUBDIR && (fd.name != std::string(".")) && (fd.name != std::string(".."))) {
				std::string childPath = filePath + fd.name + "\\";
				InitializeModel(modelBuffer, childPath);
			}

		} while (_findnext(handle, &fd) == 0);
	}
	_findclose(handle);
}

Graphics::~Graphics() {
	for (std::vector<PixelShader*>::iterator it = All_pixelShader.begin(); it != All_pixelShader.end(); it++) {
		delete *it;
	}
	for (std::vector<VertexShader*>::iterator it = All_vertexShader.begin(); it != All_vertexShader.end(); it++) {
		delete *it;
	}
}