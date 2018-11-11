#pragma once
#include "Struct.h"
class SkyBox
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11ShaderResourceView> m_SRV;
	ComPtr<ID3D11SamplerState> samp;
	unsigned int m_indexCount;
	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;

public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void LoadSkyBoxTexture(std::string name);
	void initializeShape(float size = 1);
	void draw(float x, float y, float z);
};

