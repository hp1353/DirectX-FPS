#pragma once
class cCamera
{
protected:
	/*D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookAt;
	D3DXVECTOR3		m_vUp;
	D3DXVECTOR3		m_vDir;*/

	D3DXVECTOR3*	m_pvTarget;

	POINT			m_ptPrevMouse;
	bool			m_isLButtonDown;
	float			m_fCameraDistance;

	D3DXVECTOR3		m_vShakePos;

	SYNTHESIZE(float, m_fShakePow, ShakePow);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3,	m_vEyeTrans, EyeTrans);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vLookAtTrans, LookAtTrans);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCamRotAngle, CamRotAngle);
	
	SYNTHESIZE(D3DXVECTOR3,	m_vEye, Eye);
	SYNTHESIZE(D3DXVECTOR3,	m_vLookAt, LookAt);
	SYNTHESIZE(D3DXVECTOR3,	m_vUp, Up);
	SYNTHESIZE(D3DXVECTOR3,	m_vDir, Dir);

	float AspectRatio;	//화면 종횡비
	float Near;			//카메라 Near
	float Far;			//카메라 Far
	float FOV;			//카메라 화각 ( 라디안 )

	bool Ortho;			//이게 false 이면 원근 투영, true 이면 직교투영
	float OrthoSize;	//직교크기 세로크기가 됨


	D3DXMATRIXA16 matView;				//뷰 행렬
	D3DXMATRIXA16 matProj;		//투영 행렬
	D3DXMATRIXA16 matViewProjection;	//뷰 * 투영 행렬




	D3DXMATRIXA16	m_matWorld;


public:
	cCamera(void);
	virtual ~cCamera(void);

	virtual void Setup(D3DXVECTOR3* pvTarget = NULL);
	virtual void Update();
	virtual void UpdateLightCam();
	virtual void UpdateToDevice();
	virtual void ShakeUpdate();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

