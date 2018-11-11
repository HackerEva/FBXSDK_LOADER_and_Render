#pragma once
#include "../FBXLoader/FBXExportDATA.h"
#include <fstream>
#include<string>
#include"Utilities.h"
#include<d3d11.h>
#pragma comment(lib,"FBXLoader.lib")
class Material
{
public:
	Material();
	~Material();
	void LoadMaterial(char* path);
	const wchar_t* GetDiffuseTexture();
	const wchar_t* GetSpecularTexture();
	const wchar_t* GetEmissiveTexture();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT4 GetSpecularColor();
	DirectX::XMFLOAT4 GetEmissiveColor();
	float GetDiffuseFactor();
	float GetSpecularFactor();
	float GetEmissiveFactor();
private:
	FBXExportDATA::material_t material;
};




