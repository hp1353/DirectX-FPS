#include "stdafx.h"
#include "cMapCamera.h"


cMapCamera::cMapCamera()
{
}


cMapCamera::~cMapCamera()
{
}

void cMapCamera::Setup(D3DXVECTOR3 * pvTarget)
{
	m_vEye = D3DXVECTOR3(0, 0, -50);
	m_vLookAt = D3DXVECTOR3(0, 0, 0);
	m_vCamRotAngle = D3DXVECTOR3(D3DX_PI / 4, 0, 0);

	m_vPosition = D3DXVECTOR3(0, 0, 0);

	cCamera::Setup(pvTarget);
}

void cMapCamera::Update()
{
	CameraController();

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;

	m_vEye = D3DXVECTOR3(0, 0, -m_fCameraDistance);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	if (m_pvTarget)
	{
		m_vLookAt = *m_pvTarget;
		m_vEye = m_vEye + *m_pvTarget;
	}
	else
	{
		m_vLookAt = m_vPosition;
		m_vEye = m_vEye + m_vPosition;
	}
	
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);

	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cMapCamera::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_RBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;
	case WM_RBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
	{
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);

		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);
			float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
			float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;
			m_vCamRotAngle.y += (fDeltaX / 100.f);
			m_vCamRotAngle.x += (fDeltaY / 100.f);
			if (m_vCamRotAngle.x < -D3DX_PI / 2.0f + 0.0001f)
				m_vCamRotAngle.x = -D3DX_PI / 2.0f + 0.0001f;
			if (m_vCamRotAngle.x > D3DX_PI / 2.0f - 0.0001f)
				m_vCamRotAngle.x = D3DX_PI / 2.0f - 0.0001f;
			m_ptPrevMouse = ptCurrMouse;
		}
	}
	break;
	case WM_MOUSEWHEEL:
		m_fCameraDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 30.f);
		if (m_fCameraDistance < 1.f)
			m_fCameraDistance = 1.f;
		if (m_fCameraDistance > 70.f)
			m_fCameraDistance = 70.f;
		break;
	}
}

void cMapCamera::CameraController()
{
	D3DXVECTOR3 dir(0, 0, 1);
	D3DXMATRIXA16 matR;
	D3DXMatrixRotationY(&matR, m_vCamRotAngle.y);
	D3DXVec3TransformCoord(&dir, &dir, &matR);

	if (g_pKeyManager->IsStayKeyDown('W'))
	{
		m_vPosition += dir * 0.5f;
	}
	else if (g_pKeyManager->IsStayKeyDown('S'))
	{
		m_vPosition -= dir * 0.5f;
	}
	
	D3DXVECTOR3 vUp(0, 1, 0);
	D3DXVec3Cross(&dir, &dir, &vUp);

	if (g_pKeyManager->IsStayKeyDown('A'))
	{
		m_vPosition += dir * 0.5f;
	}
	else if (g_pKeyManager->IsStayKeyDown('D'))
	{
		m_vPosition -= dir * 0.5f;
	}

}

POINT cMapCamera::GetMousePosition()
{
	return m_ptMouse;
}
