#pragma once
#include <iostream>
#include <ctime>
#include "XTime.h"
using namespace std;
#include <vector>
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

class LoadModel
{
private:
	// Cached pointer to device resources.
	DeviceResources m_deviceResources;

	// Direct3D resources for cat geometry.
	ID3D11InputLayout	m_inputLayout;
	ID3D11Buffer		m_vertexBuffer;
	ID3D11Buffer		m_indexBuffer;
	ID3D11VertexShader	m_vertexShader;
	ID3D11PixelShader m_pixelShader;
	ID3D11Buffer		m_constantBuffer;
	ID3D11ShaderResourceView m_catdiff;
	ID3D11ShaderResourceView m_catnorm;
	ID3D11ShaderResourceView m_catspec;

	ID3D11Buffer m_lightd;
	ID3D11Buffer m_lightp;
	ID3D11Buffer m_lights;

	ID3D11BlendState m_blend;

	// System resources for cat geometry.
	ModelViewProjectionConstantBuffer	m_constantBufferData;
	ModelViewProjectionConstantBuffer	m_constantBufferData2;
	uint32	m_indexCount;

	//light
	DirectionalLightConstantBuffer m_dcfd;
	PointLightConstantBuffer m_pcfd;
	SpotLightConstantBuffer m_scfd;

	// Variables used with the rendering loop.
	bool	m_loadingComplete;
	float	m_degreesPerSecond;
	bool	m_tracking;

	// Data members for keyboard and mouse input
	char	m_kbuttons[256];
	Windows::UI::Input::PointerPoint^ m_currMousePos;
	Windows::UI::Input::PointerPoint^ m_prevMousePos;

	// Matrix data member for the camera
	DirectX::XMFLOAT4X4 m_camera;
	DirectX::XMFLOAT4X4 m_camera2;
	int num;


	const char *path;
	string path2;
	string path3;
	string path4;
	int n;
	int loadmodel;
};