#include "Shape.h"

ID3D11Device* Shape::dev = nullptr;
ID3D11DeviceContext* Shape::con = nullptr;

void Shape::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Shape::initializeShape(float size)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	transform._44 = 1;
	VertexPositionUVNormal Plane[] =
	{
		{XMFLOAT3(-1, 0.0f, 1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 0.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f), {0.25f,0.25f ,0.25f ,0.25f },{0,0,0,0}  },
		{XMFLOAT3(1, 0.0f, 1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 0.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f) ,{ 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 } },
		{XMFLOAT3(-1, 0.0f, -1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 1.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f),{ 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 } },
		{XMFLOAT3(1, 0.0f, -1),XMFLOAT3(0.0f, 1.0f, 0.0f),XMFLOAT3(-1.0f, 1.0f, 1),XMFLOAT4(1.0f, 0.0f, 0.0f,0.0f) ,{ 0.25f,0.25f ,0.25f ,0.25f },{ 0,0,0,0 } },
	};

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(Plane);
	desc.StructureByteStride = sizeof(VertexPositionUVNormal);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = Plane;

	dev->CreateBuffer(&desc, &source, vertexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());

}

void Shape::draw()
{
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST


	con->Draw(4, 0);
}
