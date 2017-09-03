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

	float AspectRatio;	//ȭ�� ��Ⱦ��
	float Near;			//ī�޶� Near
	float Far;			//ī�޶� Far
	float FOV;			//ī�޶� ȭ�� ( ���� )

	bool Ortho;			//�̰� false �̸� ���� ����, true �̸� ��������
	float OrthoSize;	//����ũ�� ����ũ�Ⱑ ��


	D3DXMATRIXA16 matView;				//�� ���
	D3DXMATRIXA16 matProj;		//���� ���
	D3DXMATRIXA16 matViewProjection;	//�� * ���� ���




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

