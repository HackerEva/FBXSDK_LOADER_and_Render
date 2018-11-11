#pragma once
#include <fbxsdk.h>
#include "FBXExportDATA.h"
#include <set>
#include <fstream>

class FBXLoader
{
public:
	struct FBXVertex
	{
		XMFLOAT3 vertex;
		XMFLOAT3 normal;
		XMFLOAT3 uv;
		XMFLOAT4 tangent;
		std::vector<float> blendWeight;
		std::vector<int> blendIndices;
	};

	struct FBXTriangle
	{
		FBXVertex v[3];
	};

	struct FBXJoint
	{
		XMFLOAT4X4 bindposinverse;
	};

	struct JointInfluence
	{
		FbxCluster* joint;
		int jointindex;
		float weight;

		JointInfluence(FbxCluster* _joint, int _index, float _weight) :joint(_joint), jointindex(_index), weight(_weight) {}
	};

	struct JointInfluences
	{
		std::vector<JointInfluence> influences;

		JointInfluences()
		{
			influences.reserve(8);
		}

		void NormalizeWeights()
		{
			float totalWeight = 0;
			int i = 0;
			for (i = 0; i < (int)influences.size(); i++)
			{
				totalWeight += influences[i].weight;
			}
			float invTotalWeight = 1.0f / totalWeight;
			for (i = 0; i < (int)influences.size(); i++)
			{
				influences[i].weight *= invTotalWeight;
			}
		}
	};
private:
	static std::set<FbxNode*> vaildbone;
public:
	static void LoadFBX(char* path, FBXExportDATA* sdata);
private:
	static void ProcessNode(FbxNode* pNode, FBXExportDATA* sdata, FbxScene* pScene);
	static void ProcessMesh(FbxNode* pNode, FBXExportDATA* sdata);
	static void ProcessSkeleton(FbxNode* pNode, FBXExportDATA* sdata);
	static void ProcessKeyframes(FBXExportDATA* sdata, FbxScene* pScene);
	static void ProcessAnimation(FbxNode* pNode, FbxNode* parent, FbxTimeSpan* timeSpan, FbxAnimLayer* layer, FBXExportDATA* sdata);
	static void ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, XMFLOAT3* pVertex);
	static void ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, XMFLOAT3* pUV);
	static void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3* pNormal);
	static void ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, XMFLOAT4* pTangent);
	static void ProcessMaterial(FBXExportDATA * sdata, FbxSurfaceMaterial* sfMaterial);
	static void WriteBinary(FBXExportDATA* sdata, char* path);
	/*
	0:is Defualt
	1:is Modified
	2:is texture
	*/
	static void WriteMaterialBinary(FBXExportDATA * sdata, char * path);
	static int  isDefualt(uint16_t index);
};
//extern FBX_API FBXLoader fbxflie;

