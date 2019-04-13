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
	cb_ps_light.data.dynamicLightColor = light.lightColor;
	cb_ps_light.data.dynamicLightStrength = light.lightStrength;
	cb_ps_light.data.dynamicLightPosition = light.GetPositionFloat3();
	cb_ps_light.data.dynamicLightAttenuation_a = light.attenuation_a;
	cb_ps_light.data.dynamicLightAttenuation_b = light.attenuation_b;
	cb_ps_light.data.dynamicLightAttenuation_c = light.attenuation_c;
	cb_ps_light.ApplyChanges();
	deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_light.GetAddressOf());

	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	deviceContext->IASetInputLayout(this->vertexshader.GetInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->RSSetState(this->rasterizerState.Get());
	deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF);
	deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

	
	
	{
		const DirectX::XMMATRIX viewProjectionMatrix = Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix();
		deviceContext->VSSetShader(this->All_vertexShader[0]->GetShader(), NULL, 0);
		deviceContext->PSSetShader(this->All_pixelShader[0]->GetShader(), NULL, 0);
		deviceContext->IASetInputLayout(this->All_vertexShader[0]->GetInputLayout());
		gameObject.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());

		deviceContext->VSSetShader(this->All_vertexShader[0]->GetShader(), NULL, 0);
		deviceContext->PSSetShader(this->All_pixelShader[5]->GetShader(), NULL, 0);
		deviceContext->IASetInputLayout(this->All_vertexShader[0]->GetInputLayout());
		sampleObj2.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());

		for (std::vector<GameObject_v2*>::iterator it = all_gameObj->begin(); it != all_gameObj->end(); it++) {
			deviceContext->VSSetShader(this->All_vertexShader[(*it)->renderer.vertexshaderID]->GetShader(), NULL, 0);
			deviceContext->PSSetShader(this->All_pixelShader[(*it)->renderer.pixelshaderID]->GetShader(), NULL, 0);
			deviceContext->IASetInputLayout(this->All_vertexShader[(*it)->renderer.vertexshaderID]->GetInputLayout());
			(*it)->renderer.Renderer_Draw(viewProjectionMatrix);
		}
	}
	{
		deviceContext->PSSetShader(pixelshader_nolight.GetShader(), NULL, 0);
		light.Draw(Camera3D.GetViewMatrix() * Camera3D.GetProjectionMatrix());
	}
	
	//Draw text + ������ ����
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	float tmp = fpsTimer.GetMilisecondsElapsed();
	float dt = fpsTimer.Time.deltaTime;
	fpsTimer.timeinfo_Update();
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

	this->swapchain->Present(1, NULL); //vsync ����, 1�̸� ������ ����ȭ, 0�̸� ������ �ִ�
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	try{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapters found.");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC scd = { 0 };

		scd.BufferDesc.Width = this->windowWidth;
		scd.BufferDesc.Height = this->windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, //IDXGI Adapter
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL, //FOR SOFTWARE DRIVER TYPE
			NULL, //FLAGS FOR RUNTIME LAYERS
			NULL, //FEATURE LEVELS ARRAY
			0, //# OF FEATURE LEVELS IN ARRAY
			D3D11_SDK_VERSION,
			&scd, //Swapchain description
			this->swapchain.GetAddressOf(), //Swapchain Address
			this->device.GetAddressOf(), //Device Address
			NULL, //Supported feature level
			this->deviceContext.GetAddressOf()); //Device Context Address

		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed.");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create render target view.");

		//Describe our Depth/Stencil Buffer + ������ Ȱ���ؼ� �ڵ� ���̱�(���ʿ� default �з����� ���� �� �ǵ���� ��)
		CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->windowWidth, this->windowHeight);
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil buffer.");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil view.");

		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		//Create depth stencil state ���ٽ� & ����
		CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
		depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create depth stencil state.");

		//���ٽ� draw mask
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

		//���ٽ� apply mask
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

		//����Ʈ ����� & ����
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));
		this->deviceContext->RSSetViewports(1, &viewport);

		//�����Ͷ����� state ����
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//cull front �����Ͷ����� state ����
		CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create rasterizer state.");

		//Create Blend state
		D3D11_BLEND_DESC blendDesc = { 0 };

		D3D11_RENDER_TARGET_BLEND_DESC rtbd = { 0 };
		//blend alpha : Final alpha = source alpha * source alpha factor +(blend Operation) destination alpha * destination alpha factor

		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; //Blend operation
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = rtbd;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create blend state.");

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
		//�ӽ� �׽�Ʈ��
		std::string cat_filepath = "Data\\Objects\\Test_cat\\12221_Cat_v1_l3.obj";
		std::string nanosuit_filepath = "Data\\Objects\\nanosuit\\nanosuit.obj";

		//�ؽ��� �ҷ�����
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

		//�� ������ �ʱ�ȭ
		if (!gameObject.Initialize("Data\\Objects\\nanosuit\\nanosuit.obj", this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader)) {
			MessageBoxA(NULL, "Initialize Model Error", "Error", MB_ICONERROR);
			return false;
		}

		if (!sampleObj2.Initialize("Data\\Objects\\Test_cat\\12221_Cat_v1_l3.obj", this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader)) {
MessageBoxA(NULL, "Initialize Model Error", "Error", MB_ICONERROR);
return false;
		}

		//���� ������ �ʱ�ȭ
		if (!light.Initialize(this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexshader)) {
			MessageBoxA(NULL, "Initialize light Error", "Error", MB_ICONERROR);
			return false;
		}

		//��������Ʈ ����
		if (!sprite.Initialize(this->device.Get(), this->deviceContext.Get(), 256, 256, "Data/Textures/circle.png", cb_vs_vertexshader_2d))
		{
			MessageBoxA(NULL, "Initialize sprite Error", "Error", MB_ICONERROR);
			return false;
		}
		//sprite.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		sprite.SetPosition(XMFLOAT3(windowWidth / 2 - sprite.GetWidth() / 2, windowHeight / 2 - sprite.GetHeight() / 2, 0.0f));

		Camera2D.SetProjectionValues(windowWidth, windowHeight, 0.0f, 1.0f);

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

void Graphics::Find_Shader_File(std::wstring & shaderfolder) //���߿� ���̴� �ʱ�ȭ �Լ��� ��ĥ ��
{
	//���� �ȿ� hlsl���� Ȯ�� �� 
	//std::string path = "hlsl\\*.hlsl"; // hlsl ���� ���. ���� ������Ʈ ���ϰ� ���� ���� �ȿ� �ִٰ� ����.
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

	UINT numElements3D = ARRAYSIZE(layout3D);

	//vertex shader �ʱ�ȭ
	if ((handle = _findfirst(vertexShader_path.c_str(), &fd)) != -1L) {
		do {
			shader_filename = StringHelper::EraseFileExtension(fd.name);

			VertexShader *vs = new VertexShader;
			if (!vs->Initialize(this->device, shaderfolder + StringHelper::StringToWide(shader_filename + ".cso"), layout3D, numElements3D, shader_filename))
				MessageBoxA(NULL, "Shader Initialize error.", ERROR, MB_ICONERROR);
			All_vertexShader.push_back(vs);


		} while (_findnext(handle, &fd) == 0);
	}

	//pixel shader �ʱ�ȭ
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

	struct _finddata_t fd, fd_fbx, fd_obj;
	intptr_t handle, handle_fbx, handle_obj;

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