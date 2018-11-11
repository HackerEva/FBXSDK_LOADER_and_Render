#pragma once
#include "Struct.h"
class Camera
{
private:
	XMFLOAT4X4 transform;
	XMFLOAT4X4 proj;
	WORD mouseX;
	WORD mouseY;
	ComPtr<ID3D11Buffer> constantBuffer;
	static ID3D11Device* dev;
	static ID3D11DeviceContext* con;
private:
	void Forward(float speed);
	void Strafe(float speed);
	void Fly(float speed);
	void Yaw(float speed);
	void Pitch(float speed);
public:
	static void InitDevice(ID3D11Device* _dev, ID3D11DeviceContext* _con);
	Camera();
	~Camera();
	void InitCamera();
	void SetProjection(float FOV, float width, float height, float nearZ, float farZ);
	void Update(float dt);
	void OnMouseDown(WPARAM btnState, WORD x, WORD y);
	void OnMouseUp(WPARAM btnState, WORD x, WORD y);
	void OnMouseMove(WPARAM btnState, WORD x, WORD y);
	float DegreeToRadian(float rad);
	XMMATRIX GetPos()const;
	XMMATRIX GetProjectionMatrix()const;
	XMMATRIX GetViewMatrix()const;
	XMMATRIX GetViewProjectionMatrix()const;
	XMMATRIX GetViewMatrixInverse()const;
};

