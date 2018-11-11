#include "Shader.h"
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

ID3D11Device* Shader::dev = nullptr;
ID3D11DeviceContext* Shader::con = nullptr;

void Shader::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

void Shader::SetGroundShader()
{
	con->VSSetShader(gvshader.Get(), 0, 0);
	con->PSSetShader(gpshader.Get(), 0, 0);
	con->IASetInputLayout(layout.Get());
}

void Shader::SetCommonShader()
{
	con->VSSetShader(commonshader.Get(), 0, 0);
	con->PSSetShader(gpshader.Get(), 0, 0);
	con->IASetInputLayout(layout.Get());
}

void Shader::SetLightShader()
{
	con->VSSetShader(commonshader.Get(), 0, 0);
	con->PSSetShader(lightshader.Get(), 0, 0);
	con->IASetInputLayout(layout.Get());
}

void Shader::SetSkyBoxShader()
{
	con->VSSetShader(skyVShader.Get(), 0, 0);
	con->PSSetShader(skyPShader.Get(), 0, 0);
	con->IASetInputLayout(layout.Get());
}

void Shader::Init()
{
	D3D_SHADER_MACRO macro[] = { nullptr, nullptr };
	ID3DBlob *blobshader = 0;
	ID3DBlob *error = 0;
	UINT flag = 0;
#ifdef _DEBUG
	flag |= D3DCOMPILE_DEBUG;
	flag |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	HRESULT hr = D3DCompileFromFile(L"Trivial_VS.hlsl", macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", flag, 0, &blobshader, &error);
	HR(hr);

	hr = dev->CreateVertexShader(blobshader->GetBufferPointer(), blobshader->GetBufferSize(), 0, gvshader.GetAddressOf());
	HR(hr);

	D3D11_INPUT_ELEMENT_DESC elements[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	hr = dev->CreateInputLayout(elements, ARRAYSIZE(elements),
		blobshader->GetBufferPointer(), blobshader->GetBufferSize(), layout.GetAddressOf());

	blobshader->Release();
	if (error)
		error->Release();

	hr = D3DCompileFromFile(L"Trivial_PS.hlsl", macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", flag, 0, &blobshader, &error);
	HR(hr);

	hr = dev->CreatePixelShader(blobshader->GetBufferPointer(), blobshader->GetBufferSize(), 0, gpshader.GetAddressOf());
	HR(hr);

	blobshader->Release();
	if (error)
		error->Release();
	
	hr = D3DCompileFromFile(L"CommonVertexShader.hlsl", macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", flag, 0, &blobshader, &error);
	HR(hr);

	hr = dev->CreateVertexShader(blobshader->GetBufferPointer(), blobshader->GetBufferSize(), 0, commonshader.GetAddressOf());
	HR(hr);

	blobshader->Release();
	if (error)
		error->Release();

	hr = D3DCompileFromFile(L"LightShader.hlsl", macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", flag, 0, &blobshader, &error);
	HR(hr);

	hr = dev->CreatePixelShader(blobshader->GetBufferPointer(), blobshader->GetBufferSize(), 0, lightshader.GetAddressOf());
	HR(hr);

	blobshader->Release();
	if (error)
		error->Release();

	hr = D3DCompileFromFile(L"SkyBoxVertexShader.hlsl", macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0", flag, 0, &blobshader, &error);
	HR(hr);

	hr = dev->CreateVertexShader(blobshader->GetBufferPointer(), blobshader->GetBufferSize(), 0, skyVShader.GetAddressOf());
	HR(hr);

	blobshader->Release();
	if (error)
		error->Release();
	hr = D3DCompileFromFile(L"SkyBoxPixelShader.hlsl", macro, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0", flag, 0, &blobshader, &error);
	HR(hr);

	hr = dev->CreatePixelShader(blobshader->GetBufferPointer(), blobshader->GetBufferSize(), 0, skyPShader.GetAddressOf());
	HR(hr);

	blobshader->Release();
	if (error)
		error->Release();
}
