#pragma once
#include "cCamera.h"
class cMapCamera : public cCamera
{
	D3DXVECTOR3		m_vPosition;
	POINT			m_ptMouse;

public:
	cMapCamera();
	virtual ~cMapCamera();

	virtual void Setup(D3DXVECTOR3* pvTarget = NULL);
	virtual void Update();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CameraController();
	POINT GetMousePosition();
};

