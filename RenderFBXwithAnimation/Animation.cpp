#include "Animation.h"
#include "Joint.h"

void Animation::initializeBinaryAnimation(const char * path, Joint * _joint)
{
	joint = _joint;
	ifstream file(path, ios::in | ios::binary | ios::ate);
	file.seekg(0, ios::beg);
	UINT num;
	file.read((char*)&num, sizeof(UINT));
	std::vector<VertexPositionUVNormal> TriangleVertexList;

	for (unsigned int i = 0; i < num; i++)
	{
		VertexPositionUVNormal vertex1;
		file.read((char*)&vertex1.pos, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.normal, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.uv, sizeof(XMFLOAT3));

		file.read((char*)&vertex1.tangent, sizeof(XMFLOAT4));
		file.read((char*)&vertex1.blendIndices, sizeof(XMINT4));
		file.read((char*)&vertex1.blendWeight, sizeof(XMFLOAT4));
	}

	file.read((char*)&num, sizeof(UINT));
	BindPosition BindList;
	//file.read((char*)&BindList.pos, sizeof(XMFLOAT4));
	for (unsigned int i = 0; i < num; i++)
	{
		file.read((char*)&BindList.pos[i], sizeof(XMFLOAT4X4));
	}


	file.read((char*)&totaltime, sizeof(float));
	file.read((char*)&framerate, sizeof(float));
	file.read((char*)&framerate_inv, sizeof(float));
	file.read((char*)&num, sizeof(UINT));
	for (int i = 0; i < (int)num; i++)
	{
		std::vector<KeyFrame> frames;
		//int numOfKeyFrames = _fbxflie->keys[i].size();
		UINT numOfKeyFrames;
		file.read((char*)&numOfKeyFrames, sizeof(UINT));
		totalKeyframes = numOfKeyFrames;
		for (int j = 0; j < (int)numOfKeyFrames; j++)
		{
			KeyFrame newFrames;
			//newFrames.pose = _fbxflie->keys[i][j];
			//newFrames.time = _fbxflie->keytime[i][j];
			file.read((char*)&newFrames.pose, sizeof(XMFLOAT4X4));
			file.read((char*)&newFrames.time, sizeof(float));
			frames.push_back(newFrames);
		}
		keyframes.push_back(frames);
	}
	file.close();
}

void Animation::sentToJoint(int _key)
{
	PosList list;
	for (int i = 0; i < (int)keyframes.size(); i++)
	{
		XMMATRIX m0;
		m0 = XMLoadFloat4x4(&keyframes[i][_key].pose);
		XMStoreFloat4x4(&list.pose[i], XMLoadFloat4x4(&joint->BindList.pos[i])  * m0);
	}
	joint->poselist = list;
}

void Animation::Interpolate(float delta)
{
	PosList list;
	for (int i = 0; i < (int)keyframes.size(); i++)
	{
		XMVECTOR trans, rot, scale;
		XMVECTOR trans1, rot1, scale1;
		XMMATRIX m0, m1;
		int key = 0;
		m0 = XMLoadFloat4x4(&keyframes[i][key].pose);
		if (keyframes[i].size() > 1)
		{
			while (1)
			{
				if (key + 1 >= (int)keyframes[i].size())
				{
					key = 0;
					currtime = 0;
					break;
				}
				if (currtime >= keyframes[i][key].time && currtime < keyframes[i][key + 1].time)
				{
					break;
				}
				key++;
			}
			m0 = XMLoadFloat4x4(&keyframes[i][key].pose);
			m1 = XMLoadFloat4x4(&keyframes[i][key + 1].pose);
			float interval = keyframes[i][key + 1].time - keyframes[i][key].time;
			float ratio = (currtime - keyframes[i][key].time) / interval;
			XMMatrixDecompose(&scale, &rot, &trans, m0);
			XMMatrixDecompose(&scale1, &rot1, &trans1, m1);

			trans = XMVectorLerp(trans, trans1, ratio);
			rot = XMQuaternionNormalize(XMQuaternionSlerp(rot, rot1, ratio));
			scale = XMVectorLerp(scale, scale1, ratio);
			m0 = XMMatrixAffineTransformation(scale, XMQuaternionIdentity(), rot, trans);


		}
		XMStoreFloat4x4(&list.pose[i],XMLoadFloat4x4(&joint->BindList.pos[i])  * m0);
	}
	joint->poselist = list;
	currtime += delta;
}
