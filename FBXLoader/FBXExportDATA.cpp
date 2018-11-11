#include "FBXExportDATA.h"
#include "FBXLoader.h"
void FBXExportDATA::LoadFBX(char * path)
{
	FBXLoader::LoadFBX(path, this);
}

void FBXExportDATA::AddVertex(const XMFLOAT3 & v)
{
	Outvertex.push_back(v);
}

void FBXExportDATA::AddNormal(const XMFLOAT3 & n)
{
	Outnormal.push_back(n);
}

void FBXExportDATA::AddUv(const XMFLOAT3 & uv)
{
	Outuv.push_back(uv);
}

void FBXExportDATA::AddTangent(const XMFLOAT4 & t)
{
	Outtangent.push_back(t);
}

void FBXExportDATA::AddJoint(const XMFLOAT4X4 & v)
{
	OutJoint.push_back(v);
}

void FBXExportDATA::AddWeight(const XMFLOAT4 & v)
{
	OutWeight.push_back(v);
}

void FBXExportDATA::AddBoneIndex(const XMINT4 & v)
{
	OutBoneIndex.push_back(v);
}

void FBXExportDATA::SetFrameRate(const float & v)
{
	framerate = v;
	framerate_inv = 1.0f / v;
}

void FBXExportDATA::SetTotalTime(const float & v)
{
	animationTime = v;
}

const XMFLOAT3 * FBXExportDATA::GetVertex() const
{
	//std::vector<XMFLOAT3> vertex;
	//for (int i = 0; i < Outvertex.size(); i++)
	//{
	//	XMFLOAT3 newv;
	//	newv.x = Outvertex[i].x;
	//	newv.y = Outvertex[i].y;
	//	newv.z = Outvertex[i].z * -1;
	//	vertex.push_back(newv);
	//}
	//return vertex.data();
	return Outvertex.data();
}

const XMFLOAT3 * FBXExportDATA::GetNormal() const
{
	//std::vector<XMFLOAT3> normal;
	//for (int i = 0; i < Outnormal.size(); i++)
	//{
	//	XMFLOAT3 newv;
	//	newv.x = Outnormal[i].x;
	//	newv.y = Outnormal[i].y;
	//	newv.z = Outnormal[i].z * -1;
	//	normal.push_back(newv);
	//}
	////return Outvertex.data();
	//return normal.data();
	return Outnormal.data();
}

const XMFLOAT3 * FBXExportDATA::GetUv() const
{
	return Outuv.data();
}

const XMFLOAT4 * FBXExportDATA::GetTangent() const
{
	//std::vector<XMFLOAT4> tangent;
	//for (int i = 0; i < Outtangent.size(); i++)
	//{
	//	XMFLOAT4 newv;
	//	newv.x = Outtangent[i].x;
	//	newv.y = Outtangent[i].y;
	//	newv.z = Outtangent[i].z * -1;
	//	newv.w = Outtangent[i].w;
	//	tangent.push_back(newv);
	//}
	////return Outvertex.data();
	//return tangent.data();
	return Outtangent.data();
}

const XMFLOAT4X4 * FBXExportDATA::GetJoint() const
{
	//std::vector<XMFLOAT4X4> joint;
	//joint = OutJoint;
	//for (int i = 0; i < joint.size(); i++)
	//{
	//	joint[i].m[0][2] = joint[i].m[0][2] * -1;
	//	joint[i].m[1][2] = joint[i].m[1][2] * -1;
	//	joint[i].m[2][0] = joint[i].m[2][0] * -1;
	//	joint[i].m[2][1] = joint[i].m[2][1] * -1;
	//	joint[i].m[2][3] = joint[i].m[2][3] * -1;
	//	joint[i].m[3][2] = joint[i].m[3][2] * -1;
	//}
	////return Outvertex.data();
	//return joint.data();
	return OutJoint.data();
}

const XMFLOAT4 * FBXExportDATA::GetWeight() const
{
	return OutWeight.data();
}

const XMINT4 * FBXExportDATA::GetIndex() const
{
	return OutBoneIndex.data();
}

const float FBXExportDATA::GetAnimationTime() const
{
	return animationTime;
}

const float FBXExportDATA::GetFrameRate() const
{
	return framerate;
}

const float FBXExportDATA::GetFrameRate_Inv() const
{
	return framerate_inv;
}

UINT FBXExportDATA::GetVertexSize() const
{
	return Outvertex.size();
}

UINT FBXExportDATA::GetNormalSize() const
{
	return Outnormal.size();
}

UINT FBXExportDATA::GetUvSize() const
{
	return Outuv.size();
}

UINT FBXExportDATA::GetTangentSize() const
{
	return Outtangent.size();
}

UINT FBXExportDATA::GetJointSize() const
{
	return OutJoint.size();
}

UINT FBXExportDATA::GetWeightSize() const
{
	return OutWeight.size();
}

UINT FBXExportDATA::GetIndexSize() const
{
	return OutBoneIndex.size();
}
