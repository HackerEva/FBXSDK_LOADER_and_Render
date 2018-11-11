#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
	if (material[0].input)
	{
		delete material[0].input;
	}
	if (material[1].input)
	{
		delete material[1].input;
	}
	if (material[2].input)
	{
		delete material[2].input;
	}
}

void Material::LoadMaterial(char * path)
{
	std::string name = path;
	name = name.substr(0, name.length() - 4);
	name += "_m.bin";
	std::ifstream file(name,
		std::ios::in | std::ios::binary | std::ios::ate);
	if (!file)
	{
		return;
	}

	file.seekg(0, std::ios::beg);
	for (size_t i = 0; i < FBXExportDATA::COUNT; i++)
	{
		file.read((char*)&material[i].value, sizeof(XMFLOAT4));
		file.read((char*)&material[i].factor, sizeof(float));
		size_t lenght;

		file.read((char*)&lenght, sizeof(size_t));
		if (lenght)
		{
			file.read((char*)&material[i].input, sizeof(char)*lenght);
		}
		else
		{
			material[i].input = nullptr;
		}
	}
	file.close();
}

const wchar_t * Material::GetDiffuseTexture()
{
	return Utlities::GetWC(material[0].input);
}

const wchar_t * Material::GetSpecularTexture()
{
	return Utlities::GetWC(material[1].input);
}

const wchar_t * Material::GetEmissiveTexture()
{
	return Utlities::GetWC(material[2].input);
}

DirectX::XMFLOAT4 Material::GetDiffuseColor()
{
	return material[0].value;
}

DirectX::XMFLOAT4 Material::GetSpecularColor()
{
	return material[1].value;
}

DirectX::XMFLOAT4 Material::GetEmissiveColor()
{
	return material[2].value;
}

float Material::GetDiffuseFactor()
{
	return material[0].factor;
}

float Material::GetSpecularFactor()
{
	return material[1].factor;
}

float Material::GetEmissiveFactor()
{
	return material[2].factor;
}
