#pragma once
#include"Struct.h"
#include "Shape.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Joint.h"
#include "Animation.h"
#include "DrawLight.h"
#include "SkyBox.h"
#include "Material.h"
class My3DSence
{
	ComPtr<ID3D11Device> theDevice;
	ComPtr<IDXGISwapChain> theSwapChain;
	ComPtr<ID3D11DeviceContext> theContext;

	ComPtr<ID3D11RenderTargetView> theRTV;
	D3D11_VIEWPORT theViewPort;
	ComPtr<ID3D11DepthStencilView> theDSV;
	ComPtr<ID3D11Texture2D> depthBuff;
	ComPtr<ID3D11ShaderResourceView> textureV;
	ComPtr<ID3D11ShaderResourceView> textureB;

	ComPtr<ID3D11SamplerState> binsample;
	
	ComPtr<ID3D11Texture2D> textBuff;
	HRESULT hr;

	ComPtr<ID3D11Buffer> lightd;
	ComPtr<ID3D11Buffer> lightp;
	ComPtr<ID3D11Buffer> lightpcolor;
	ComPtr<ID3D11Buffer> lights;
	ComPtr<ID3D11Buffer> lightscolor;

	DirectionalLightConstantBuffer dcfd;
	PointLightConstantBuffer pcfd;
	SpotLightConstantBuffer scfd;
	Material *bearMaterial;
	Material *boxMaterial;
	Shader shader;
	Shape shape;
	Mesh bear;
	Mesh box;
	Joint joint;
	Joint bearJoint;
	Animation animate;
	Animation bearAni;
	Camera camera;
	XTime time;
	DrawLight Plight;
	DrawLight Slight;
	SkyBox skybox;
	//ID3D11Resource *resource = nullptr;
	//ID3D11Buffer *shadercombuffer = nullptr;
	//ID3D11Buffer *gridConstBuffer = nullptr;
	bool renderBear = false;
	bool isLoopAnimation = false;
	bool drawBone = true;
	bool drawMesh = true;
	int frameBear = 0;
	int frameBox = 0;
	

	void CreateDirectionalLight();
	void CreatePointLight();
	void CreateSpotLight();
	void UpdataLight(float const moveSpd);
public:
	My3DSence();
	~My3DSence();

	bool Initialize(HWND wnd);
	bool run();
	Camera* GetCamera();
};

