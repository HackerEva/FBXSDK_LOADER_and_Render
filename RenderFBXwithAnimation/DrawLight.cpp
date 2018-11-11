#include "DrawLight.h"
ID3D11Device* DrawLight::dev = nullptr;
ID3D11DeviceContext* DrawLight::con = nullptr;

void DrawLight::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void DrawLight::initializeLigtht(float size)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	transform._44 = 1;

	static const VertexPositionUVNormal Box[] =
	{
		{ XMFLOAT3( -0.1f, -0.1f, -0.1f), XMFLOAT3(-1,0,0), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3( -0.1f, -0.1f, +0.1f), XMFLOAT3(-1,0,0), XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3( -0.1f, +0.1f, -0.1f), XMFLOAT3(-1,0,0), XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3( -0.1f, +0.1f, +0.1f), XMFLOAT3(-1,0,0), XMFLOAT3(1.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, -0.1f, -0.1f), XMFLOAT3(1,0,0) , XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, -0.1f, +0.1f), XMFLOAT3(1,0,0) , XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, +0.1f, -0.1f), XMFLOAT3(1,0,0) , XMFLOAT3(1.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, +0.1f, +0.1f), XMFLOAT3(1,0,0) , XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
							 		  							 
		{ XMFLOAT3( -0.1f, -0.1f, -0.1f), XMFLOAT3(0,-1,0), XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3( -0.1f, -0.1f, +0.1f), XMFLOAT3(0,-1,0), XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, -0.1f, -0.1f), XMFLOAT3(0,-1,0), XMFLOAT3(1.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, -0.1f, +0.1f), XMFLOAT3(0,-1,0), XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
										  							 
		{ XMFLOAT3( -0.1f, +0.1f, -0.1f), XMFLOAT3(0,1,0) , XMFLOAT3(0.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3( -0.1f, +0.1f, +0.1f), XMFLOAT3(0,1,0) , XMFLOAT3(0.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, +0.1f, -0.1f), XMFLOAT3(0,1,0) , XMFLOAT3(1.0f, 1.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
		{ XMFLOAT3(  0.1f, +0.1f, +0.1f), XMFLOAT3(0,1,0) , XMFLOAT3(1.0f, 0.0f, 0.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), { 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 }},
	};

	static const unsigned int BoxIndex[] =
	{
		0,1,2, // -x
		1,3,2,

		4,6,5, // +x
		5,6,7,

		0,6,4, // -z
		0,2,6,

		1,7,3, // +z
		1,5,7,

		8,9,11, //-y
		8,11,10,

		12,13,15, //+y
		12,15,14,
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(Box);
	desc.StructureByteStride = sizeof(VertexPositionUVNormal);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = Box;
	dev->CreateBuffer(&desc, &source, vertexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = sizeof(BoxIndex);
	desc.StructureByteStride = sizeof(unsigned int);
	source.pSysMem = BoxIndex;
	dev->CreateBuffer(&desc, &source, indexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
}

void DrawLight::draw()
{
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());
	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;

	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	con->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	con->DrawIndexed(36, 0, 0);
}

void DrawLight::TransModel(float x, float y, float z)
{
	XMStoreFloat4x4(&transform, XMMatrixTranslation(x, y, z));
}
