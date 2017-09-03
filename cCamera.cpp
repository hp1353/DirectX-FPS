#include "StdAfx.h"
#include "cCamera.h"

cCamera::cCamera(void)
	: m_vEye(0, 1.5, 0)
	, m_vLookAt(0, 1.5, 5)
	, m_vUp(0, 1, 0)
	, m_pvTarget(NULL)
	, m_fCameraDistance(50)
	, m_isLButtonDown(false)
	, m_vCamRotAngle(0, 0, 0)
	, m_vEyeTrans(0,0,0)
	, m_vLookAtTrans(0,0,0)
	, AspectRatio((float)WINSIZEX / WINSIZEY)
	, Near(0.1f)
	, Far(100)
	, FOV(D3DX_PI / 3)
	, Ortho(false)
	, OrthoSize(5.0f)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;

	D3DXMatrixIdentity(&m_matWorld);
}


cCamera::~cCamera(void)
{
}

void cCamera::Setup( D3DXVECTOR3* pvTarget )
{
	m_pvTarget = pvTarget;

	m_vDir = m_vLookAt - m_vEye;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	//RECT rc;
	//GetClientRect(g_hWnd, &rc);

	//D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,
		FOV,
		AspectRatio,
		Near, 
		Far);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Update()
{
	ShakeUpdate();

	m_vEye = D3DXVECTOR3(0, 1.5, 0) + m_vEyeTrans;
	m_vLookAt = D3DXVECTOR3(0, 1.5, 5) + m_vLookAtTrans;

	D3DXMATRIXA16 matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;
	D3DXVec3TransformCoord(&m_vLookAt, &m_vLookAt, &matR);
	D3DXVec3TransformCoord(&m_vDir, &m_vDir, &matR);

	if (m_pvTarget)
	{
		m_vEye = m_vEye + *m_pvTarget;
		m_vLookAt = m_vLookAt + *m_pvTarget;
	}

	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	


	//투영행렬 
	if (this->Ortho)
	{
		D3DXMatrixOrthoLH(
			&matProj,
			OrthoSize * this->AspectRatio,	//직교투영 가로 사이즈
			OrthoSize,					//직교투영 세로 사이즈
			this->Near,				//카메라 Near
			this->Far				//카메라 Far
		);
	}
	else
	{
		//D3DXMatrixPerspectiveFovLH 원근 투영행렬을 만든다.
		D3DXMatrixPerspectiveFovLH(
			&matProj,
			this->FOV,
			this->AspectRatio,
			this->Near,
			this->Far);
	}

	this->matViewProjection =
		this->matView * this->matProj;

}

void cCamera::UpdateLightCam()
{
	//D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	D3DXMatrixInverse(
		&matView,
		NULL,
		&m_matWorld);

	//투영행렬 
	if (this->Ortho)
	{
		D3DXMatrixOrthoLH(
			&matProj,
			OrthoSize * this->AspectRatio,	//직교투영 가로 사이즈
			OrthoSize,					//직교투영 세로 사이즈
			this->Near,				//카메라 Near
			this->Far				//카메라 Far
		);
	}
	else
	{
		//D3DXMatrixPerspectiveFovLH 원근 투영행렬을 만든다.
		D3DXMatrixPerspectiveFovLH(
			&matProj,
			this->FOV,
			this->AspectRatio,
			this->Near,
			this->Far);
	}

	this->matViewProjection =
		this->matView * this->matProj;

}

void cCamera::UpdateToDevice()
{
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::ShakeUpdate()
{
	if (m_fShakePow > 0.f)
	{
		m_vShakePos.x = (float(rand() % 6) - 3) / 10.f;
		m_vShakePos.y = (float(rand() % 6) - 3) / 10.f;
		m_vShakePos.z = (float(rand() % 6) - 3) / 10.f;

		m_fShakePow -= 0.1f;

		if(m_fShakePow < 0)
		{
			m_fShakePow = 0.f;
			m_vShakePos = D3DXVECTOR3(0, 0, 0);
		}

		m_vLookAtTrans = m_vShakePos;
	}
}

void cCamera::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;
	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		{
			if(!m_isLButtonDown)
			{
				POINT ptCurrMouse;
				ptCurrMouse.x = LOWORD(lParam);
				ptCurrMouse.y = HIWORD(lParam);
				float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
				float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;
				m_vCamRotAngle.y += (fDeltaX / 100.f);
				m_vCamRotAngle.x += (fDeltaY / 100.f);
				if(m_vCamRotAngle.x < -D3DX_PI / 4.0f + 0.0001f)
					m_vCamRotAngle.x = -D3DX_PI / 4.0f + 0.0001f;
				if(m_vCamRotAngle.x > D3DX_PI / 4.0f - 0.0001f)
					m_vCamRotAngle.x = D3DX_PI / 4.0f - 0.0001f;

				if (m_vCamRotAngle.y > D3DX_PI * 2)
					m_vCamRotAngle.y -= D3DX_PI * 2;
				if (m_vCamRotAngle.y < 0)
					m_vCamRotAngle.y += D3DX_PI * 2;
				m_ptPrevMouse = ptCurrMouse;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		m_fCameraDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 30.f);
		if(m_fCameraDistance < 0.0001f)
			m_fCameraDistance = 0.0001f;
		break;
	}
}
