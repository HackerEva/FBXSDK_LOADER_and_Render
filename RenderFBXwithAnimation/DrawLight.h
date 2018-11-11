#pragma once
#include"Struct.h"
class DrawLight
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;
	ComPtr<ID3D11Buffer> colorBuffer;
	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;
public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void initializeLigtht(float size = 1);
	void draw();
	void TransModel(float x, float y, float z);
};

