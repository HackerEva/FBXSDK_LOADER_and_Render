#pragma once
#include"Struct.h"
class Joint
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> constantBuffer;

	ComPtr<ID3D11Buffer> OffsetconstantBuffer;

	XMFLOAT4X4 transform;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;
	int vertexcount;
	std::vector<VertexPositionUVNormal> pointsforshere;
	std::vector<unsigned int> index;
	float JointsSize;
	unsigned int numOfJoint;

public:
	PosList poselist;
	BindPosition BindList;
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	void initBinaryMesh(const char * path, float size = 1, float x = 0, float y = 0, float z = 0);
	void draw(bool drawBone);
	void makesphere(float rad, int slice, int segment);
	BindPosition * GetBindPose() { return &BindList; }
};

