#pragma once
#include <iostream>
#include <ctime>
#include "XTime.h"
using namespace std;
#include <vector>
#include <fstream>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>
//#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

#include<wrl.h>
using Microsoft::WRL::ComPtr;

#define BACKBUFFER_WIDTH	1000
#define BACKBUFFER_HEIGHT	1000
#define PI		3.14159f
#define HR(hr)		assert(!FAILED(hr))
// Constant buffer used to send MVP matrices to the vertex shader.
struct CameraConstantBuffer
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT4X4 viewProj;
};

struct BindPosition
{
	XMFLOAT4X4 pos[64];
};

struct PosList
{
	XMFLOAT4X4 pose[64];
};

struct VertexPositionUVNormal
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT3 uv;
	XMFLOAT4 tangent;
	float blendWeight[4];
	int blendIndices[4];
};

struct KeyFrame
{
	float time;
	XMFLOAT4X4 pose;
};

struct DirectionalLightConstantBuffer
{
	XMFLOAT4 direction;
	XMFLOAT4 Dcolor;
	
};

struct PointLightConstantBuffer
{
	XMFLOAT4 Pointpos;
	XMFLOAT4 Pcolor;
	XMFLOAT4 lightradius;
};

struct SpotLightConstantBuffer
{
	XMFLOAT4 Spotpos;
	XMFLOAT4 Scolor;
	XMFLOAT4 conedir;
	XMFLOAT4 coneratio;
	
};