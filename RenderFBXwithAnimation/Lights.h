#pragma once
#include "Struct.h"

struct theLight
{
	ID3D11Buffer* m_lightd;
	ID3D11Buffer* m_lightp;
	ID3D11Buffer* m_lights;

	DirectionalLightConstantBuffer m_dcfd;
	PointLightConstantBuffer m_pcfd;
	SpotLightConstantBuffer m_scfd;

	void SetUp()
	{
		CD3D11_BUFFER_DESC constantBufferDescD(sizeof(DirectionalLightConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		theDevice->CreateBuffer(&constantBufferDescD, nullptr, &m_lightd);

		//CreatePointLight();
		CD3D11_BUFFER_DESC constantBufferDescP(sizeof(PointLightConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		theDevice->CreateBuffer(&constantBufferDescP, nullptr, &m_lightp);

		//CreateSpotLight();
		CD3D11_BUFFER_DESC constantBufferDescS(sizeof(SpotLightConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		theDevice->CreateBuffer(&constantBufferDescS, nullptr, &m_lights);

		CreateDirectionalLight();
		CreatePointLight();
		CreateSpotLight();

		
	}

	void Render()
	{
		theContext->UpdateSubresource(m_lightd, 0, NULL, &m_dcfd, 0, 0);
		theContext->UpdateSubresource(m_lightp, 0, NULL, &m_pcfd, 0, 0);
		theContext->UpdateSubresource(m_lights, 0, NULL, &m_scfd, 0, 0);

		theContext->PSSetConstantBuffers(0, 1, &m_lightd);
		theContext->PSSetConstantBuffers(1, 1, &m_lightp);
		theContext->PSSetConstantBuffers(2, 1, &m_lights);
	}

	void CreateDirectionalLight()
	{
		m_dcfd.direction = { -5.0f,2.0f,0.0f,0.0f };
		m_dcfd.Dcolor = { 1.0f,1.0f,1.0f,1.0f };
	}
	void CreatePointLight()
	{
		m_pcfd.Pointpos = { 0.0f,0.0f,-1.0f,0.0f };
		m_pcfd.Pcolor = { 0.0f,0.0f,1.0f,0.0f };
		m_pcfd.lightradius = { 0.5f,0.0f,0.0f,0.0f };
	}
	void CreateSpotLight()
	{
		m_scfd.Spotpos = { 0.0f,2.0f,0.0f,0.0f };
		m_scfd.Scolor = { 1.0f,0.0f,0.0f,0.0f };
		m_scfd.conedir = { 0.0f,-1.0f,0.0f,0.0f };
		m_scfd.coneratio = { 0.8f,0.0f,0.0f,0.0f };
	}

	void UpdataLight(float const moveSpd)
	{
		//directional:
		if (GetAsyncKeyState('I') && GetAsyncKeyState('1'))
		{
			m_dcfd.direction.z += moveSpd;
		}
		if (GetAsyncKeyState('K') && GetAsyncKeyState('1'))
		{
			m_dcfd.direction.z += -moveSpd;
		}
		if (GetAsyncKeyState('J') && GetAsyncKeyState('1'))
		{
			m_dcfd.direction.x += -moveSpd;
		}
		if (GetAsyncKeyState('L') && GetAsyncKeyState('1'))
		{
			m_dcfd.direction.x += moveSpd;
		}
		if (GetAsyncKeyState('U') && GetAsyncKeyState('1'))
		{
			m_dcfd.direction.y += -moveSpd;
		}
		if (GetAsyncKeyState('O') && GetAsyncKeyState('1'))
		{
			m_dcfd.direction.y += moveSpd;
		}

		//pointlight
		if (GetAsyncKeyState('I') && GetAsyncKeyState('2'))
		{
			m_pcfd.Pointpos.z += moveSpd;
		}
		if (GetAsyncKeyState('K') && GetAsyncKeyState('2'))
		{
			m_pcfd.Pointpos.z += -moveSpd;
		}
		if (GetAsyncKeyState('J') && GetAsyncKeyState('2'))
		{
			m_pcfd.Pointpos.x += -moveSpd;
		}
		if (GetAsyncKeyState('L') && GetAsyncKeyState('2'))
		{
			m_pcfd.Pointpos.x += moveSpd;
		}
		if (GetAsyncKeyState('U') && GetAsyncKeyState('2'))
		{
			m_pcfd.Pointpos.y += -moveSpd;
		}
		if (GetAsyncKeyState('O') && GetAsyncKeyState('2'))
		{
			m_pcfd.Pointpos.y += moveSpd;
		}

		//Spotlight pos
		if (GetAsyncKeyState('I') && GetAsyncKeyState('3'))
		{
			m_scfd.Spotpos.z += moveSpd;
		}
		if (GetAsyncKeyState('K') && GetAsyncKeyState('3'))
		{
			m_scfd.Spotpos.z += -moveSpd;
		}
		if (GetAsyncKeyState('J') && GetAsyncKeyState('3'))
		{
			m_scfd.Spotpos.x += -moveSpd;
		}
		if (GetAsyncKeyState('L') && GetAsyncKeyState('3'))
		{
			m_scfd.Spotpos.x += moveSpd;
		}
		if (GetAsyncKeyState('U') && GetAsyncKeyState('3'))
		{
			m_scfd.Spotpos.y += -moveSpd;
		}
		if (GetAsyncKeyState('O') && GetAsyncKeyState('3'))
		{
			m_scfd.Spotpos.y += moveSpd;
		}

		//Spotlight dir
		if (GetAsyncKeyState('I') && GetAsyncKeyState('4'))
		{
			m_scfd.conedir.z += moveSpd;
		}
		if (GetAsyncKeyState('K') && GetAsyncKeyState('4'))
		{
			m_scfd.conedir.z += -moveSpd;
		}
		if (GetAsyncKeyState('J') && GetAsyncKeyState('4'))
		{
			m_scfd.conedir.x += -moveSpd;
		}
		if (GetAsyncKeyState('L') && GetAsyncKeyState('4'))
		{
			m_scfd.conedir.x += moveSpd;
		}
		if (GetAsyncKeyState('U') && GetAsyncKeyState('4'))
		{
			m_scfd.conedir.y += -moveSpd;
		}
		if (GetAsyncKeyState('O') && GetAsyncKeyState('4'))
		{
			m_scfd.conedir.y += moveSpd;
		}
	}
	void release()
	{
		/*m_lightd->Release();
		m_lightp->Release();
		m_lights->Release();*/
	}
};