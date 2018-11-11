#include "Joint.h"
ID3D11Device* Joint::dev = nullptr;
ID3D11DeviceContext* Joint::con = nullptr;

void Joint::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Joint::initBinaryMesh(const char * path, float size, float x, float y, float z)
{
	ifstream file(path, ios::in | ios::binary | ios::ate);
	file.seekg(0, ios::beg);
	UINT num;
	file.read((char*)&num, sizeof(UINT));
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
	}

	file.read((char*)&num, sizeof(UINT));
	numOfJoint = num;
	JointsSize = size;
	//file.read((char*)&BindList.pos, sizeof(XMFLOAT4));
	for (unsigned int i = 0; i < num; i++)
	{
		file.read((char*)&BindList.pos[i], sizeof(XMFLOAT4X4));
	}

	file.close();

	XMStoreFloat4x4(&transform, XMMatrixIdentity());
	D3D11_BUFFER_DESC desc = {};

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	dev->CreateBuffer(&desc, 0, OffsetconstantBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(XMFLOAT4X4) * 64;

	dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
	makesphere(0.1f, 6, 6);
}

void Joint::draw(bool drawBone)
{
	D3D11_MAPPED_SUBRESOURCE maps;

	if (drawBone)
	{
		unsigned int stride = sizeof(VertexPositionUVNormal);
		UINT offset = 0;

		con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		con->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (int i = 0; i < (int)numOfJoint; i++)
		{
			XMMATRIX m = XMLoadFloat4x4(&poselist.pose[i]);

			XMMATRIX inv = XMLoadFloat4x4(&BindList.pos[i]);
			m = XMMatrixInverse(nullptr, inv) * m;
			XMVECTOR t, r, s;
			XMMatrixDecompose(&s, &r, &t, m);
			float scale = JointsSize* JointsSize;
			t = XMVectorScale(t, scale);

			m = XMMatrixAffineTransformation(s, XMQuaternionIdentity(), r, t);

			XMStoreFloat4x4(&transform, m);

			con->Map(OffsetconstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
			memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
			con->Unmap(OffsetconstantBuffer.Get(), 0);
			con->VSSetConstantBuffers(1, 1, OffsetconstantBuffer.GetAddressOf());
			con->DrawIndexed((UINT)index.size(), 0, 0);
		}
	}

	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &poselist, sizeof(XMFLOAT4X4) * numOfJoint);
	con->Unmap(constantBuffer.Get(), 0);
	con->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());

	////D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	////D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
}

void Joint::makesphere(float rad, int slice, int segment)
{
	float stepY = XM_PI / segment;
	float stepX = XM_2PI / slice;

	VertexPositionUVNormal startpoint;
	startpoint.pos = XMFLOAT3(0.0f, rad, 0.0f);
	pointsforshere.push_back(startpoint);
	VertexPositionUVNormal endpoint;
	endpoint.pos = XMFLOAT3(0.0f, -rad, 0.0f);
	float Xstart = 0;
	float Ystart = 0 + stepY;
	for (int j = 0; j < segment; j++)
	{
		for (int i = 0; i < slice; i++)
		{
			VertexPositionUVNormal newpoint;
			newpoint.pos.x = rad*std::sin(Xstart)*std::sin(Ystart);
			newpoint.pos.y = rad*std::cos(Ystart);
			newpoint.pos.z = rad*std::cos(Xstart)*std::sin(Ystart);

			Xstart += stepX;
			pointsforshere.push_back(newpoint);
		}

		Xstart = 0;
		Ystart += stepY;
	}
	pointsforshere.push_back(endpoint);

	unsigned int rowA = 0;
	unsigned int rowB = 1;
	unsigned int i, j;
	for (i = 0; i < (UINT)slice - 1; i++)
	{
		index.push_back(rowA);
		index.push_back(rowB + i);
		index.push_back(rowB + i + 1);
	}

	index.push_back(rowA);
	index.push_back(rowB + i);
	index.push_back(rowB);

	for (j = 1; j < (UINT)segment - 1; j++)
	{
		rowA = 1 + (j - 1)*segment;
		rowB = rowA + slice;

		for (i = 0; i < (UINT)slice - 1; i++)
		{
			index.push_back(rowA + i);
			index.push_back(rowB + i);
			index.push_back(rowA + i + 1);

			index.push_back(rowA + i + 1);
			index.push_back(rowB + i);
			index.push_back(rowB + i + 1);
		}

		index.push_back(rowA + i);
		index.push_back(rowB + i);
		index.push_back(rowA);

		index.push_back(rowA);
		index.push_back(rowB + i);
		index.push_back(rowB);
	}

	rowA = 1 + (segment - 2)*slice;
	rowB = rowA + slice;

	for (i = 0; i < (UINT)slice - 1; i++)
	{
		index.push_back(rowA + i);
		index.push_back(rowB);
		index.push_back(rowA + i + 1);
	}

	index.push_back(rowA + i);
	index.push_back(rowB);
	index.push_back(rowA);

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = pointsforshere.size() * sizeof(VertexPositionUVNormal);
	desc.StructureByteStride = sizeof(VertexPositionUVNormal);
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA source = {};
	source.pSysMem = &pointsforshere[0];
	dev->CreateBuffer(&desc, &source, vertexBuffer.GetAddressOf());

	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = index.size() * sizeof(unsigned int);
	desc.StructureByteStride = sizeof(unsigned int);
	source.pSysMem = &index[0];
	dev->CreateBuffer(&desc, &source, indexBuffer.GetAddressOf());
}
