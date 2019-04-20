#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include <vector>
#include <io.h>
#include <string>
#include <iostream>
#include "Camera3D.h"
#include "Camera2D.h"
#include "..\\Timer.h"
#include "..\\StringHelper.h"
#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"
#include "RenderableGameObject.h"
#include "../Component/GameObject_v2.h"
#include "../SceneManager.h"
#include "Light.h"
#include "Sprite.h"

#include <vector>

class Graphics {
public:
	bool Initialize(HWND hwnd, int width, int height);
	void InitializeModel(ModelBuffer & modelBuffer);
	void InitializeModel(ModelBuffer & modelBuffer, std::string & path);
	bool InitializeScene();
	void RenderFrame();
	~Graphics();

	Camera3D Camera3D;
	Camera2D Camera2D;
	Sprite sprite;
	Light light;

	//Test
	std::vector<GameObject_v2*> * all_gameObj;

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	

	void Find_Shader_File(std::wstring & shaderfolder);
	

	Microsoft::WRL::ComPtr<ID3D11Device> device; //디바이스 인터페이스 : 기능 지원 점검과 자원 할당에 쓰임
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext; //디바이스 컨텍스트 인터페이스 : 렌더 대상을 설정하고 자원을 그래픽 파이프라인에 묶고 Gpu가 수행할 렌더링 명령들을 지시하는데 쓰인다.
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain; //프론트 버퍼 백 버퍼 바꿔치기
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexshader;
	VertexShader vertexshader_2d;
	PixelShader pixelshader;
	PixelShader pixelshader_2d;
	PixelShader pixelshader_2d_discard;
	PixelShader pixelshader_nolight;
	ConstantBuffer<CB_VS_vertexshader_2d> cb_vs_vertexshader_2d;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_light> cb_ps_light;

	std::vector<VertexShader*> All_vertexShader;
	std::vector<PixelShader*> All_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_drawMask;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState_applyMask;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> grassTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pavementTexture;

	int windowWidth = 0;
	int windowHeight = 0;
	bool mEnable4xMSAA = false;

	Timer fpsTimer;
};