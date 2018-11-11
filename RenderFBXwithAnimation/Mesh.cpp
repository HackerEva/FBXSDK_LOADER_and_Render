#include "Mesh.h"

ID3D11Device* Mesh::dev = nullptr;
ID3D11DeviceContext* Mesh::con = nullptr;

void Mesh::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Mesh::initBinaryMesh(const char * path, float size, float x, float y, float z)
{
	XMStoreFloat4x4(&transform, XMMatrixIdentity()*size);
	transform._44 = 1;
	ifstream file(path, ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		int xx = 100;
		xx = 1;
	}
	file.seekg(0, ios::beg);
	UINT num;
	file.read((char*)&num, sizeof(UINT));
	
	XMMATRIX mFix = XMMatrixSet(-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1);
	std::vector<VertexPositionUVNormal> TriangleVertexList;

	for (unsigned int i = 0; i < num; i++)
	{
		VertexPositionUVNormal vertex1;
		file.read((char*)&vertex1.pos, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.normal, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.uv, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.tangent, sizeof(XMFLOAT4));
		file.read((char*)&vertex1.blendIndices, sizeof(XMINT4));
		file.read((char*)&vertex1.blendWeight, sizeof(XMFLOAT4));
		

		TriangleVertexList.push_back(vertex1);
	}
	file.close();
	vertexcount = TriangleVertexList.size();

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = vertexcount * sizeof(VertexPositionUVNormal);
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = &TriangleVertexList[0];

	dev->CreateBuffer(&desc, &source, vertexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	dev->CreateBuffer(&desc, 0, scaleBuffer.GetAddressOf());

	modelsize.x = size;
}

void Mesh::draw(bool drawMesh)
{
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

	con->Map(scaleBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &modelsize, sizeof(float));
	con->Unmap(scaleBuffer.Get(), 0);

	con->VSSetConstantBuffers(4, 1, scaleBuffer.GetAddressOf());

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	if (drawMesh)
		con->Draw(vertexcount, 0);
}


