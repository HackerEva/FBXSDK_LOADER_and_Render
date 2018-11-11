#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <assert.h>
#include <cstdint>
#include <bitset>
#include <array>

using namespace DirectX;
//
#define EXPORT
#if defined EXPORT
#define FBX_API __declspec(dllexport)
#else
#define FBX_API __declspec(dllimport)
#endif
class FBX_API FBXExportDATA
{
private:
	std::vector<XMFLOAT3> Outvertex;
	std::vector<XMFLOAT3> Outnormal;
	std::vector<XMFLOAT3> Outuv;
	std::vector<XMFLOAT4> Outtangent;
	std::vector<XMFLOAT4X4> OutJoint;
	std::vector<XMFLOAT4> OutWeight;
	std::vector<XMINT4> OutBoneIndex;

	float framerate;
	float framerate_inv;
	float animationTime;
	//XMFLOAT4X4 transL;

public:
	void LoadFBX(char* path);
	uint64_t DefaultIdentity;
	std::vector<std::vector<XMFLOAT4X4>> keys;
	std::vector<std::vector<float>> keytime;
	/*
	0:is Defualt
	1:is Modified
	2:is texture
	*/
	enum ProprityType {
		Number,
		Bool,
		Vector
	};
	enum components { DIFFUSE = 0, SPECULAR, EMISSIVE, COUNT };


	union source_t
	{
		uint64_t id;
		const wchar_t* file_path;
		const void* texture_resource;
	};

	struct component_t
	{
		XMFLOAT4 value;
		float factor;
		char* input;
	};
	struct material_t : std::array< component_t, components::COUNT >
	{
		std::bitset<64> pipeline_id;

		using std::array< component_t, components::COUNT >::array;
	};
	material_t Material;

	uint64_t  isDefualt(uint16_t index);
	uint64_t mPow2(uint64_t right);
	void AddVertex(const XMFLOAT3& v);
	void AddNormal(const XMFLOAT3& n);
	void AddUv(const XMFLOAT3& uv);
	void AddTangent(const XMFLOAT4& t);
	void AddJoint(const XMFLOAT4X4& v);
	void AddWeight(const XMFLOAT4& v);
	void AddBoneIndex(const XMINT4& v);
	void SetFrameRate(const float& v);
	void SetTotalTime(const float& v);

	const XMFLOAT3* GetVertex()const;
	const XMFLOAT3* GetNormal()const;
	const XMFLOAT3* GetUv()const;
	const XMFLOAT4* GetTangent()const;
	const XMFLOAT4X4* GetJoint()const;
	const XMFLOAT4* GetWeight()const;
	const XMINT4* GetIndex()const;
	const float GetAnimationTime()const;
	const float GetFrameRate()const;
	const float GetFrameRate_Inv()const;

	UINT GetVertexSize()const;
	UINT GetNormalSize()const;
	UINT GetUvSize()const;
	UINT GetTangentSize()const;
	UINT GetJointSize()const;
	UINT GetWeightSize()const;
	UINT GetIndexSize()const;
};