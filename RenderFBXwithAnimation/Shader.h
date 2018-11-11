#pragma once
#include "Struct.h"

class Shader
{
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;

	ComPtr<ID3D11PixelShader> gpshader;
	ComPtr<ID3D11VertexShader> gvshader;

	ComPtr<ID3D11VertexShader> skyVShader;
	ComPtr<ID3D11PixelShader> skyPShader;

	ComPtr<ID3D11VertexShader> commonshader;
	ComPtr<ID3D11PixelShader> lightshader;

	ComPtr<ID3D11InputLayout> layout;

public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void SetGroundShader();
	void SetCommonShader();
	void SetLightShader();
	void SetSkyBoxShader();
	void Init();
};

