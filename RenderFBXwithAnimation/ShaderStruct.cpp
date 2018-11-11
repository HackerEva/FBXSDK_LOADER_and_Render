#pragma once

namespace DX11UWA
{
	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model[4];
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4 CameraWorldPos;
		//DirectX::XMFLOAT4X4 lightViewMatrix;
		//DirectX::XMFLOAT4X4 lightProjectionMatrix;
	};

	// Used to send per-vertex data to the vertex shader.
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	struct VertexPositionUVNormal
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 uv;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 tangent;
	};

	struct DirectionalLightConstantBuffer
	{
		DirectX::XMFLOAT4 direction;
		DirectX::XMFLOAT4 Dcolor;
	};

	struct PointLightConstantBuffer
	{
		DirectX::XMFLOAT4 Pointpos;
		DirectX::XMFLOAT4 Pcolor;
		DirectX::XMFLOAT4 lightradius;
	};

	struct SpotLightConstantBuffer
	{
		DirectX::XMFLOAT4 Spotpos;
		DirectX::XMFLOAT4 Scolor;
		DirectX::XMFLOAT4 conedir;
		DirectX::XMFLOAT4 coneratio;
	};
}