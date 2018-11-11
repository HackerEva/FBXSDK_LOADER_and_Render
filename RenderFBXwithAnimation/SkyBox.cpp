#include "SkyBox.h"
#include "DDSTextureLoader.h"

ID3D11Device* SkyBox::dev = nullptr;
ID3D11DeviceContext* SkyBox::con = nullptr;
void SkyBox::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void SkyBox::LoadSkyBoxTexture(std::string name)
{
	std::wstring temp = std::wstring(name.begin(), name.end());
	const wchar_t* path = temp.c_str();
	CreateDDSTextureFromFile(dev, path,
		NULL, m_SRV.GetAddressOf());
}

void SkyBox::initializeShape(float size)
{
	XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity()*size);
	transform._44 = 1;
	LoadSkyBoxTexture("SunsetSkybox.dds");
	float s = 200.0f;
	static const VertexPositionUVNormal cubeVertices[] =
			{
		/*0*/   { XMFLOAT3(-s, -s, -s), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(-0.377964497f, -0.377964497f, -0.377964497f) },
		/*1*/   { XMFLOAT3(-s, -s,  s), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(-0.377964497f, -0.377964497f,  0.377964497f) },
		/*2*/   { XMFLOAT3(-s,  s, -s), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(-0.377964497f,  0.377964497f, -0.377964497f) },
		/*3*/	{ XMFLOAT3(-s,  s,  s), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(-0.377964497f,  0.377964497f,  0.377964497f) },
		/*4*/	{ XMFLOAT3(s,  -s, -s), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.377964497f, -0.377964497f, -0.377964497f) },
		/*5*/	{ XMFLOAT3(s,  -s,  s), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.377964497f, -0.377964497f,  0.377964497f) },
		/*6*/	{ XMFLOAT3(s,   s, -s), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.377964497f,  0.377964497f, -0.377964497f) },
		/*7*/	{ XMFLOAT3(s,   s,  s), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.377964497f,  0.377964497f,  0.377964497f) },
		/*8*/   { XMFLOAT3(-s, -s, -s), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(-0.377964497f, -0.377964497f, -0.377964497f) },
		/*9*/	{ XMFLOAT3(-s, -s,  s), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(-0.377964497f, -0.377964497f,  0.377964497f) },
		/*10*/	{ XMFLOAT3(-s,  s, -s), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(-0.377964497f,  0.377964497f, -0.377964497f) },
		/*11*/	{ XMFLOAT3(-s,  s,  s), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(-0.377964497f,  0.377964497f,  0.377964497f) },
		/*12*/	{ XMFLOAT3(s,  -s, -s), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT3(0.377964497f, -0.377964497f, -0.377964497f) },
		/*13*/	{ XMFLOAT3(s,  -s,  s), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.377964497f, -0.377964497f,  0.377964497f) },
		/*14*/	{ XMFLOAT3(s,   s, -s), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.377964497f,  0.377964497f, -0.377964497f) },
		/*15*/	{ XMFLOAT3(s,   s,  s), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT3(0.377964497f,  0.377964497f,  0.377964497f) },

			};
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
			vertexBufferData.pSysMem = cubeVertices;
			vertexBufferData.SysMemPitch = 0;
			vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
			dev->CreateBuffer(&vertexBufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());

			static const unsigned short cubeIndices[] =
			{
				10,9,8, // -x
				10,11,9,

				13,14,12, // +x
				15,14,13,

				1,5,0, // -y
				5,4,0,

				6,7,2, // +y
				7,3,2,

				4,6,0, // -z
				6,2,0,

				3,7,1, // +z
				7,5,1,
				/////////

			};
			m_indexCount = ARRAYSIZE(cubeIndices);
			D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
			
			indexBufferData.pSysMem = cubeIndices;
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
			dev->CreateBuffer(&indexBufferDesc, &indexBufferData, indexBuffer.GetAddressOf());

			D3D11_BUFFER_DESC desc = {};
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.ByteWidth = sizeof(XMFLOAT4X4);
			desc.StructureByteStride = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.Usage = D3D11_USAGE_DYNAMIC;

			dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());

			D3D11_SAMPLER_DESC sdesc = {};
			sdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			dev->CreateSamplerState(&sdesc, samp.GetAddressOf());
}

void SkyBox::draw(float x, float y, float z)
{
	transform._41 = x;
	transform._42 = y;
	transform._43 = z;
	D3D11_MAPPED_SUBRESOURCE maps;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &maps);
	memcpy(maps.pData, &transform, sizeof(XMFLOAT4X4));
	con->Unmap(constantBuffer.Get(), 0);

	unsigned int stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	con->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	con->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	con->VSSetConstantBuffers(1, 1, constantBuffer.GetAddressOf());

	con->PSSetSamplers(0, 1, samp.GetAddressOf());
	
	ID3D11ShaderResourceView* Views[] = { m_SRV.Get() };
	con->PSSetShaderResources(0, 1, Views);
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST


	con->DrawIndexed(m_indexCount, 0, 0);
}
