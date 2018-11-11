#include"FBXLoader.h"
std::set<FbxNode*> FBXLoader::vaildbone;

void FBXLoader::LoadFBX(char* path, FBXExportDATA* sdata)
{
	sdata->DefaultIdentity = 0;

	FbxManager* FbxSdkManager = nullptr;
	if (FbxSdkManager == nullptr)
	{
		FbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(FbxSdkManager, IOSROOT);
		FbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(FbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(FbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(path, -1, FbxSdkManager->GetIOSettings());
	bSuccess = pImporter->Import(pFbxScene);
	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		ProcessNode(pFbxRootNode, sdata, pFbxScene);
	}

	ProcessKeyframes(sdata, pFbxScene);
	WriteBinary(sdata, path);
	vaildbone.clear();
	if (pFbxScene->GetMaterialCount())
	{
		ProcessMaterial(sdata, pFbxScene->GetMaterial(0));
		WriteMaterialBinary(sdata, path);
	}

	FbxSdkManager->Destroy();
}

void FBXLoader::ProcessNode(FbxNode* pNode, FBXExportDATA* sdata, FbxScene* pScene)
{
	FbxNodeAttribute* attributeType = pNode->GetNodeAttribute();
	if (attributeType)
	{
		switch (attributeType->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			ProcessMesh(pNode, sdata);
			break;
		case FbxNodeAttribute::eSkeleton:
			ProcessSkeleton(pNode, sdata);
			break;
		case FbxNodeAttribute::eLight:
			//ProcessLight(pNode);
			break;
		case FbxNodeAttribute::eCamera:
			//ProcessCamera();
			break;
		}
	}

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessNode(pNode->GetChild(i), sdata, pScene);
	}
}

void FBXLoader::ProcessMesh(FbxNode* pNode, FBXExportDATA* sdata)
{
	FbxMesh* pMesh = pNode->GetMesh();
	if (pMesh == nullptr)
		return;

	FbxSkin* skin = (FbxSkin*)pNode->GetMesh()->GetDeformer(0, FbxDeformer::eSkin);
	int boneCount = skin->GetClusterCount();
	int triangleCount = pMesh->GetPolygonCount();
	int vertexCounter = 0;
	FbxVector4* ctrlPoints = 0;
	ctrlPoints = pMesh->GetControlPoints();
	int cpcount = pMesh->GetControlPointsCount();
	std::vector<JointInfluences> inf;
	inf.resize(cpcount);
	for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
	{
		FbxCluster* cluster = skin->GetCluster(boneIndex);
		FbxNode* bone = cluster->GetLink();

		vaildbone.insert(bone);

		FbxAMatrix bindPoseMatrix;

		XMFLOAT4X4 newJoint;
		FbxAMatrix newbindpose;

		cluster->GetTransformLinkMatrix(bindPoseMatrix);
		cluster->GetTransformMatrix(newbindpose);

		newbindpose = bindPoseMatrix.Inverse() * newbindpose;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				newJoint.m[row][col] = (float)newbindpose.Get(row, col);
			}
		}

		int *boneVertexIndices = cluster->GetControlPointIndices();
		double *boneVertexWeights = cluster->GetControlPointWeights();
		int numBoneVertexIndices = cluster->GetControlPointIndicesCount();


		for (int boneVertexIndex = 0; boneVertexIndex < numBoneVertexIndices; boneVertexIndex++)
		{
			int cpIndex = boneVertexIndices[boneVertexIndex];
			float boneWeight = (float)boneVertexWeights[boneVertexIndex];
			inf[cpIndex].influences.push_back(JointInfluence(cluster, boneIndex, boneWeight));
		}

		//change:
		newJoint.m[0][2] = newJoint.m[0][2] * -1;
		newJoint.m[1][2] = newJoint.m[1][2] * -1;
		newJoint.m[2][0] = newJoint.m[2][0] * -1;
		newJoint.m[2][1] = newJoint.m[2][1] * -1;
		newJoint.m[2][3] = newJoint.m[2][3] * -1;
		newJoint.m[3][2] = newJoint.m[3][2] * -1;
		sdata->AddJoint(newJoint);
	}

	for (int i = 0; i < triangleCount; ++i)
	{
		int polySize = pMesh->GetPolygonSize(i);
		assert(polySize == 3);
		FBXTriangle newTriangle;
		for (int j = 0; j < 3; j++)
		{
			int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);

			newTriangle.v[j].vertex.x = (float)ctrlPoints[ctrlPointIndex][0];
			newTriangle.v[j].vertex.y = (float)ctrlPoints[ctrlPointIndex][1];
			newTriangle.v[j].vertex.z = (float)ctrlPoints[ctrlPointIndex][2];

			newTriangle.v[j].vertex.z *= -1;//change

											// Read the vertex  
											//ReadVertex(pMesh, ctrlPointIndex, &newTriangle.v[j].vertex);
			for (int iff = 0; iff < (int)inf[ctrlPointIndex].influences.size(); iff++)
			{
				newTriangle.v[j].blendIndices.push_back(inf[ctrlPointIndex].influences[iff].jointindex);
				newTriangle.v[j].blendWeight.push_back(inf[ctrlPointIndex].influences[iff].weight);
			}
			// Read the UV of each vertex  
			for (int k = 0; k < 2; ++k)
			{
				ReadUV(pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), k, &newTriangle.v[j].uv);
			}

			// Read the normal of each vertex  
			ReadNormal(pMesh, ctrlPointIndex, vertexCounter, &newTriangle.v[j].normal);

			// Read the tangent of each vertex  
			ReadTangent(pMesh, ctrlPointIndex, vertexCounter, &newTriangle.v[j].tangent);

			vertexCounter++;
		}
		std::swap(newTriangle.v[1], newTriangle.v[2]);
		/*XMVECTOR v0 = XMLoadFloat3(&newTriangle.v[0].vertex);
		XMVECTOR v1 = XMLoadFloat3(&newTriangle.v[1].vertex);
		XMVECTOR v2 = XMLoadFloat3(&newTriangle.v[2].vertex);
		XMVECTOR v01 = v1 - v0;
		XMVECTOR v02 = v2 - v0;
		if (XMVectorGetX(XMVector3Dot(v01, v02)) < 0)
		std::swap(newTriangle.v[1], newTriangle.v[2]);*/

		//auto& Outvertex = *sdata->GetVertex();
		sdata->AddVertex(newTriangle.v[0].vertex);
		sdata->AddVertex(newTriangle.v[1].vertex);
		sdata->AddVertex(newTriangle.v[2].vertex);

		sdata->AddNormal(newTriangle.v[0].normal);
		sdata->AddNormal(newTriangle.v[1].normal);
		sdata->AddNormal(newTriangle.v[2].normal);

		sdata->AddUv(newTriangle.v[0].uv);
		sdata->AddUv(newTriangle.v[1].uv);
		sdata->AddUv(newTriangle.v[2].uv);

		sdata->AddTangent(newTriangle.v[0].tangent);
		sdata->AddTangent(newTriangle.v[1].tangent);
		sdata->AddTangent(newTriangle.v[2].tangent);



		for (int ti = 0; ti < 3; ti++)
		{
			float bWeight[4] = { 0 };
			int bIndex[4] = { 0 };
			for (int bi = 0; bi < (int)newTriangle.v[ti].blendIndices.size(); bi++)
			{
				bIndex[bi] = newTriangle.v[ti].blendIndices[bi];
				bWeight[bi] = newTriangle.v[ti].blendWeight[bi];
			}
			sdata->AddBoneIndex(XMINT4(bIndex[0], bIndex[1], bIndex[2], bIndex[3]));
			sdata->AddWeight(XMFLOAT4(bWeight[0], bWeight[1], bWeight[2], bWeight[3]));
		}
		//pOutVertexVector.push_back(newTriangle);
	}
}

void FBXLoader::ProcessSkeleton(FbxNode* pNode, FBXExportDATA* sdata)
{

	//XMFLOAT4X4 newJoint;
	//FbxMatrix newbindpose;
	//
	//newbindpose = pNode->EvaluateGlobalTransform().Inverse();
	//for (int row = 0; row < 4; row++)
	//{
	//	for (int col = 0; col < 4; col++)
	//	{
	//		newJoint.m[row][col] = (float)newbindpose.Get(row, col);
	//	}
	//}
	////newJoint.bindposinverse = pNode->EvaluateGlobalTransform().Inverse();
	//sdata->AddJoint(newJoint);
}

void FBXLoader::ProcessKeyframes(FBXExportDATA * sdata, FbxScene* pScene)
{
	FbxTime::EMode timemode = pScene->GetGlobalSettings().GetTimeMode();
	float framerate = (float)FbxTime::GetFrameRate(timemode);
	sdata->SetFrameRate(framerate);

	FbxNode* pNode = pScene->GetRootNode();
	int stackCount = pScene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < stackCount; i++)
	{
		FbxAnimStack* stack = pScene->GetSrcObject<FbxAnimStack>(i);

		int numLayer = stack->GetMemberCount<FbxAnimLayer>();
		FbxTimeSpan timeSpan;
		pNode->GetAnimationInterval(timeSpan, stack);

		FbxTime time = timeSpan.GetStop() - timeSpan.GetStart();
		FbxLongLong ms = time.GetMilliSeconds();
		sdata->SetTotalTime((float)ms * 0.001f);
		for (int layerIndex = 0; layerIndex < numLayer; layerIndex++)
		{
			FbxAnimLayer* layer = stack->GetMember<FbxAnimLayer>(layerIndex);

			ProcessAnimation(pNode, nullptr, &timeSpan, layer, sdata);
		}
	}
}

void FBXLoader::ProcessAnimation(FbxNode * pNode, FbxNode* parent, FbxTimeSpan* timeSpan, FbxAnimLayer * layer, FBXExportDATA * sdata)
{
	FbxNodeAttribute* attributeType = pNode->GetNodeAttribute();
	if (attributeType)
	{
		if (attributeType->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			int numOfBones = sdata->GetJointSize();
			if (numOfBones >= 0)
			{
				std::vector<float> framestime;
				std::vector<XMFLOAT4X4> keys;
				int totalFrame = (int)std::ceilf(sdata->GetAnimationTime() * sdata->GetFrameRate()) + 1;
				float start = (float)timeSpan->GetStart().GetSecondDouble();

				for (int i = 0; i < totalFrame; i++)
				{
					float keytime = i * sdata->GetFrameRate_Inv();
					framestime.push_back(keytime + start);
					//EvaluateGlobalTransform
					//EvaluateLocalTransform
					FbxTime time;
					time.SetSecondDouble(start + keytime);
					FbxAMatrix m = pNode->EvaluateGlobalTransform(time);
					XMFLOAT4X4 outm;
					for (int row = 0; row < 4; row++)
					{
						for (int col = 0; col < 4; col++)
						{
							outm.m[row][col] = (float)m.Get(row, col);
						}
					}
					//change:
					outm.m[0][2] = outm.m[0][2] * -1;
					outm.m[1][2] = outm.m[1][2] * -1;
					outm.m[2][0] = outm.m[2][0] * -1;
					outm.m[2][1] = outm.m[2][1] * -1;
					outm.m[2][3] = outm.m[2][3] * -1;
					outm.m[3][2] = outm.m[3][2] * -1;

					keys.push_back(outm);
				}
				if (vaildbone.find(pNode) != vaildbone.end())
				{
					sdata->keytime.push_back(framestime);
					sdata->keys.push_back(keys);
				}
			}
		}
	}

	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessAnimation(pNode->GetChild(i), pNode, timeSpan, layer, sdata);
	}
}

void FBXLoader::ReadVertex(FbxMesh* pMesh, int ctrlPointIndex, XMFLOAT3* pVertex)
{
	FbxVector4* pCtrlPoint = pMesh->GetControlPoints();

	pVertex->x = (float)pCtrlPoint[ctrlPointIndex][0];
	pVertex->y = (float)pCtrlPoint[ctrlPointIndex][1];
	pVertex->z = (float)pCtrlPoint[ctrlPointIndex][2];
}

/*void FBXLoader::ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT4* pColor)
//{
//	if (pMesh->GetElementVertexColorCount() < 1)
//		return;

//	FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);
//	switch (pVertexColor->GetMappingMode())
//	{
//	case FbxGeometryElement::eByControlPoint:
//	{
//		switch (pVertexColor->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			pColor->x = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mRed;
//			pColor->y = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mGreen;
//			pColor->z = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mBlue;
//			pColor->w = (float)pVertexColor->GetDirectArray().GetAt(ctrlPointIndex).mAlpha;
//		}
//		break;

//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int id = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
//			pColor->x = (float)pVertexColor->GetDirectArray().GetAt(id).mRed;
//			pColor->y = (float)pVertexColor->GetDirectArray().GetAt(id).mGreen;
//			pColor->z = (float)pVertexColor->GetDirectArray().GetAt(id).mBlue;
//			pColor->w = (float)pVertexColor->GetDirectArray().GetAt(id).mAlpha;
//		}
//		break;

//		default:
//			break;
//		}
//	}
//	break;

//	case FbxGeometryElement::eByPolygonVertex:
//	{
//		switch (pVertexColor->GetReferenceMode())
//		{
//		case FbxGeometryElement::eDirect:
//		{
//			pColor->x = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mRed;
//			pColor->y = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mGreen;
//			pColor->z = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mBlue;
//			pColor->w = (float)pVertexColor->GetDirectArray().GetAt(vertexCounter).mAlpha;
//		}
//		break;
//		case FbxGeometryElement::eIndexToDirect:
//		{
//			int id = pVertexColor->GetIndexArray().GetAt(vertexCounter);
//			pColor->x = (float)pVertexColor->GetDirectArray().GetAt(id).mRed;
//			pColor->y = (float)pVertexColor->GetDirectArray().GetAt(id).mGreen;
//			pColor->z = (float)pVertexColor->GetDirectArray().GetAt(id).mBlue;
//			pColor->w = (float)pVertexColor->GetDirectArray().GetAt(id).mAlpha;
//		}
//		break;
//		default:
//			break;
//		}
//	}
//	break;
//	}
//}*/

void FBXLoader::ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, XMFLOAT3* pUV)
{
	if (uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)
		return;

	FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

	switch (pVertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pUV->x = (float)pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[0];
			pUV->y = (float)pVertexUV->GetDirectArray().GetAt(ctrlPointIndex)[1];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);
			pUV->x = (float)pVertexUV->GetDirectArray().GetAt(id)[0];
			pUV->y = (float)pVertexUV->GetDirectArray().GetAt(id)[1];
		}
		break;
		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (pVertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			pUV->x = (float)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
			pUV->y = (float)pVertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
		}
		break;
		default:
			break;
		}
	}
	break;
	}
	//pUV->x = 1 - pUV->x;
	pUV->y = 1 - pUV->y;
}

void FBXLoader::ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, XMFLOAT3* pNormal)
{
	if (pMesh->GetElementNormalCount() < 1)
		return;

	FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
	switch (leNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (leNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pNormal->x = (float)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[0];
			pNormal->y = (float)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[1];
			pNormal->z = (float)leNormal->GetDirectArray().GetAt(ctrlPointIndex)[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
			pNormal->x = (float)leNormal->GetDirectArray().GetAt(id)[0];
			pNormal->y = (float)leNormal->GetDirectArray().GetAt(id)[1];
			pNormal->z = (float)leNormal->GetDirectArray().GetAt(id)[2];
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (leNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pNormal->x = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[0];
			pNormal->y = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[1];
			pNormal->z = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = leNormal->GetIndexArray().GetAt(vertexCounter);
			pNormal->x = (float)leNormal->GetDirectArray().GetAt(id)[0];
			pNormal->y = (float)leNormal->GetDirectArray().GetAt(id)[1];
			pNormal->z = (float)leNormal->GetDirectArray().GetAt(id)[2];
		}
		break;

		default:
			break;
		}
	}
	break;
	}

	pNormal->z *= -1;//change
}

void FBXLoader::ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, XMFLOAT4* pTangent)
{
	if (pMesh->GetElementTangentCount() < 1)
		return;

	FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

	switch (leTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (leTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pTangent->x = (float)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[0];
			pTangent->y = (float)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[1];
			pTangent->z = (float)leTangent->GetDirectArray().GetAt(ctrlPointIndex)[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = leTangent->GetIndexArray().GetAt(ctrlPointIndex);
			pTangent->x = (float)leTangent->GetDirectArray().GetAt(id)[0];
			pTangent->y = (float)leTangent->GetDirectArray().GetAt(id)[1];
			pTangent->z = (float)leTangent->GetDirectArray().GetAt(id)[2];
		}
		break;

		default:
			break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (leTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			pTangent->x = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[0];
			pTangent->y = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[1];
			pTangent->z = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[2];
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int id = leTangent->GetIndexArray().GetAt(vertecCounter);
			pTangent->x = (float)leTangent->GetDirectArray().GetAt(id)[0];
			pTangent->y = (float)leTangent->GetDirectArray().GetAt(id)[1];
			pTangent->z = (float)leTangent->GetDirectArray().GetAt(id)[2];
		}
		break;

		default:
			break;
		}
	}
	break;
	}

	pTangent->z *= -1;//change
}

void FBXLoader::ProcessMaterial(FBXExportDATA * sdata, FbxSurfaceMaterial* sfMaterial)
{
	FBXExportDATA::material_t mat;
	fbxsdk::FbxDouble3 fbxdou3;
	//DIFFUSE
	fbxdou3 = ((FbxSurfaceLambert*)sfMaterial)->Diffuse;
	sdata->Material[0].value.x = (float)fbxdou3.mData[0];
	sdata->Material[0].value.y = (float)fbxdou3.mData[1];
	sdata->Material[0].value.z = (float)fbxdou3.mData[2];
	sdata->Material[0].value.w = 1;
	sdata->Material[0].factor = (float)((FbxSurfaceLambert*)sfMaterial)->DiffuseFactor;
	if (((FbxSurfaceLambert*)sfMaterial)->Diffuse.GetSrcObjectCount<FbxFileTexture>())
	{
		auto p = ((FbxSurfaceLambert*)sfMaterial)->Diffuse.GetSrcObject<FbxFileTexture>(0);
		sdata->Material[0].input = new char[64];
		strcpy_s(sdata->Material[0].input, 64, p->GetRelativeFileName());
	}
	//SPECULAR
	std::string materialName = sfMaterial->GetClassId().GetName();
	if (materialName == "FbxSurfacePhong")
	{
		fbxdou3 = ((FbxSurfacePhong*)sfMaterial)->Specular;
		sdata->Material[1].value.x = (float)fbxdou3.mData[0];
		sdata->Material[1].value.y = (float)fbxdou3.mData[1];
		sdata->Material[1].value.z = (float)fbxdou3.mData[2];
		sdata->Material[1].value.w = 1;
		sdata->Material[1].factor = (float)((FbxSurfacePhong*)sfMaterial)->SpecularFactor;
		if (((FbxSurfacePhong*)sfMaterial)->Specular.GetSrcObjectCount<FbxFileTexture>())
		{
			auto p = ((FbxSurfacePhong*)sfMaterial)->Specular.GetSrcObject<FbxFileTexture>(0);
			sdata->Material[1].input = new char[64];
			strcpy_s(sdata->Material[1].input,64, p->GetRelativeFileName());
		}
	}
	else
	{
		sdata->Material[1].value.x = 0;
		sdata->Material[1].value.y = 0;
		sdata->Material[1].value.z = 0;
		sdata->Material[1].value.w = 0;
		sdata->Material[1].factor = 0;
		sdata->Material[1].input = nullptr;
	}

	//EMISSIVE
	fbxdou3 = ((FbxSurfaceLambert*)sfMaterial)->Emissive;
	sdata->Material[2].value.x = (float)fbxdou3.mData[0];
	sdata->Material[2].value.y = (float)fbxdou3.mData[1];
	sdata->Material[2].value.z = (float)fbxdou3.mData[2];
	sdata->Material[2].value.w = 1;
	sdata->Material[2].factor = (float)((FbxSurfaceLambert*)sfMaterial)->EmissiveFactor;
	if (((FbxSurfaceLambert*)sfMaterial)->Emissive.GetSrcObjectCount<FbxFileTexture>())
	{
		auto p = ((FbxSurfaceLambert*)sfMaterial)->Emissive.GetSrcObject<FbxFileTexture>(0);
		if (p)
		{
			sdata->Material[2].input = new char[64];
			strcpy_s(sdata->Material[2].input,64, p->GetRelativeFileName());
		}
		else
		{
			sdata->Material[2].input = nullptr;
		}

	}
}

void FBXLoader::WriteBinary(FBXExportDATA * sdata, char * path)
{


	std::string name = path;
	name.replace(name.end() - 3, name.end(), "bin");
	std::ofstream file(name, std::ios::binary);
	UINT vsize = sdata->GetVertexSize();
	file.write((char*)&vsize, sizeof(UINT));

	for (int i = 0; i < (int)vsize; i++)
	{
		file.write((char*)&sdata->GetVertex()[i], sizeof(XMFLOAT3));
		file.write((char*)&sdata->GetNormal()[i], sizeof(XMFLOAT3));
		file.write((char*)&sdata->GetUv()[i], sizeof(XMFLOAT3));
		file.write((char*)&sdata->GetTangent()[i], sizeof(XMFLOAT4));
		file.write((char*)&sdata->GetIndex()[i], sizeof(XMINT4));
		file.write((char*)&sdata->GetWeight()[i], sizeof(XMFLOAT4));

	}
	vsize = sdata->GetJointSize();
	file.write((char*)&vsize, sizeof(UINT));
	const XMFLOAT4X4* Joints = sdata->GetJoint();
	for (int i = 0; i < (int)vsize; i++)
	{
		XMFLOAT4X4 tmp = Joints[i];
		file.write((char*)&tmp, sizeof(XMFLOAT4X4));
	}
	float tmp = sdata->GetAnimationTime();
	file.write((char*)&tmp, sizeof(float));
	tmp = sdata->GetFrameRate();
	file.write((char*)&tmp, sizeof(float));
	tmp = sdata->GetFrameRate_Inv();
	file.write((char*)&tmp, sizeof(float));
	vsize = (UINT)sdata->keys.size();
	file.write((char*)&vsize, sizeof(UINT));
	for (int i = 0; i < (int)vsize; i++)
	{
		UINT keynum = (UINT)sdata->keys[i].size();
		file.write((char*)&keynum, sizeof(UINT));
		for (int j = 0; j < (int)keynum; j++)
		{
			file.write((char*)&sdata->keys[i][j], sizeof(XMFLOAT4X4));
			file.write((char*)&sdata->keytime[i][j], sizeof(float));
		}
	}
	file.close();
}

void FBXLoader::WriteMaterialBinary(FBXExportDATA * sdata, char * path)
{
	std::string name = path;
	name = name.substr(0, name.length() - 4);
	name += "_m.bin";
	std::ofstream file(name, std::ios::binary);
	for (size_t i = 0; i < FBXExportDATA::COUNT; i++)
	{
		//Value
		file.write((char*)&sdata->Material[i].value, sizeof(XMFLOAT4));
		file.write((char*)&sdata->Material[i].factor, sizeof(float));
		size_t lenght = 0;
		if (sdata->Material[i].input)
		{
			size_t lenght = strlen(sdata->Material[i].input);
			file.write((char*)&lenght, sizeof(size_t));
			file.write((char*)&sdata->Material[i].input, sizeof(char)*lenght);
		}
		else
		{
			file.write((char*)&lenght, sizeof(size_t));
		}

	}
	file.close();
}
