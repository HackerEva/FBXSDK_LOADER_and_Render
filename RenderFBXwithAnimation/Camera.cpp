#include "Camera.h"

ID3D11Device* Camera::dev = nullptr;
ID3D11DeviceContext* Camera::con = nullptr;


void Camera::InitDevice(ID3D11Device * _dev, ID3D11DeviceContext * _con)
{
	dev = _dev;
	con = _con;
}

Camera::Camera() : mouseX(0), mouseY(0)
{
	XMMATRIX view = XMMatrixIdentity();
	view.r[3] = XMVectorSet(0, 4, -4, 1);
	//view = XMMatrixRotationX(0.5f) * XMMatrixRotationY(XM_PI) * view;
	XMStoreFloat4x4(&transform, view);
}

Camera::~Camera()
{
}

void Camera::InitCamera()
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(CameraConstantBuffer);
	desc.StructureByteStride = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	HRESULT hr = dev->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
	HR(hr);
}

void Camera::SetProjection(float FOV, float width, float height, float nearZ, float farZ)
{
	float aspectRatio = width / height;
	XMStoreFloat4x4(&proj, XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearZ, farZ));
}

void Camera::Update(float dt)
{
	if (GetAsyncKeyState(VK_SHIFT))dt *= 10;
	if (GetAsyncKeyState('W'))
	{
		Forward(dt);
	}

	if (GetAsyncKeyState('S'))
	{
		Forward(-dt);
	}

	if (GetAsyncKeyState('A'))
	{
		Strafe(-dt);
	}

	if (GetAsyncKeyState('D'))
	{
		Strafe(dt);
	}


	if (GetAsyncKeyState(VK_SPACE))
	{
		Fly(dt);
	}


	if (GetAsyncKeyState(VK_CONTROL))
	{
		Fly(-dt);
	}

	CameraConstantBuffer ccb;
	XMStoreFloat4x4(&ccb.view, GetViewMatrix());
	XMStoreFloat4x4(&ccb.projection, GetProjectionMatrix());
	XMStoreFloat4x4(&ccb.viewProj, GetViewProjectionMatrix());

	D3D11_MAPPED_SUBRESOURCE mr;
	con->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mr);
	memcpy(mr.pData, &ccb, sizeof(CameraConstantBuffer));
	con->Unmap(constantBuffer.Get(), 0);
	con->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}

void Camera::OnMouseDown(WPARAM btnState, WORD x, WORD y)
{
	mouseX = x;
	mouseY = y;
}

void Camera::OnMouseUp(WPARAM btnState, WORD x, WORD y)
{
	mouseX = x;
	mouseY = y;
}

void Camera::OnMouseMove(WPARAM btnState, WORD x, WORD y)
{
	if (btnState & VK_LBUTTON)
	{
		float dx = DegreeToRadian(x - (float)mouseX);
		float dy = DegreeToRadian(y - (float)mouseY);
		Yaw(dx);
		Pitch(dy);
	}
	
	mouseX = x;
	mouseY = y;
}

float Camera::DegreeToRadian(float rad)
{
	return rad*0.017453293f;
}

void Camera::Forward(float speed)
{
	XMVECTOR vel = XMVectorSet(0, 0, speed, 1);
	XMMATRIX mat = XMLoadFloat4x4(&transform);
	XMVECTOR pos = mat.r[3];

	mat.r[3] = XMVectorSet(0, 0, 0, 1);
	vel = XMVector3Transform(vel, mat);
	vel.m128_f32[3] = 0;
	mat.r[3] = pos + vel;
	XMStoreFloat4x4(&transform, mat);
}

void Camera::Strafe(float speed)
{
	XMVECTOR vel = XMVectorSet(speed, 0, 0, 1);
	XMMATRIX mat = XMLoadFloat4x4(&transform);
	XMVECTOR pos = mat.r[3];

	mat.r[3] = XMVectorSet(0, 0, 0, 1);
	vel = XMVector3Transform(vel, mat);
	vel.m128_f32[3] = 0;
	mat.r[3] = pos + vel;
	XMStoreFloat4x4(&transform, mat);

}

void Camera::Fly(float speed)
{
	XMVECTOR vel = XMVectorSet(0, speed, 0, 1);
	XMMATRIX mat = XMLoadFloat4x4(&transform);
	XMVECTOR pos = mat.r[3];

	mat.r[3] = XMVectorSet(0, 0, 0, 1);
	vel = XMVector3Transform(vel, mat);
	vel.m128_f32[3] = 0;
	mat.r[3] = pos + vel;
	XMStoreFloat4x4(&transform, mat);

}

void Camera::Yaw(float speed)
{
	XMMATRIX rotY = XMMatrixRotationY(speed);
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	XMVECTOR pos = trans.r[3];
	trans.r[3] = XMVectorSet(0, 0, 0, 1);
	trans = trans*rotY;
	trans.r[3] = pos;
	XMStoreFloat4x4(&transform, trans);

}

void Camera::Pitch(float speed)
{
	XMMATRIX rotX = XMMatrixRotationX(speed);
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	trans = rotX*trans;
	XMStoreFloat4x4(&transform, trans);

}

XMMATRIX Camera::GetPos() const
{
	XMMATRIX trans = XMLoadFloat4x4(&transform);
	XMMATRIX pos = XMMatrixTranslationFromVector(trans.r[3]);
	return pos;
}

XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&proj);
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMMatrixInverse(0,(XMLoadFloat4x4(&transform)));
}

XMMATRIX Camera::GetViewProjectionMatrix() const
{
	return XMMatrixInverse(0, (XMLoadFloat4x4(&transform))) * XMLoadFloat4x4(&proj);
}

XMMATRIX Camera::GetViewMatrixInverse() const
{
	return XMLoadFloat4x4(&transform);
}

