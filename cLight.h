#pragma once

#include "cTexQuad.h"
#include "cCamera.h"
#include "cRenderTexture.h"

class cLight
{
public:
	static LPD3DXEFFECT pLightEffect;		//����Ʈ Effect

public:
	D3DXMATRIXA16 Transform;		//������ Transform
	D3DXCOLOR lightColor;		//���� ����
	float intensity;			//������ ����
	D3DXVECTOR3 Dir;

private:
	cTexQuad		directnalQuad;		//���⼺ ������ �׸��� ���� ��ũ������

	cCamera			lightCamera;		//���⼺ ������ �׸��� ���� �׸��� ���� ī�޶�...
	cRenderTexture*	pShadowMap;			//���̰��� ������ ������ ��

	float			shadowDistance;

public:
	cLight();
	~cLight();

	//Texture �� �����Ѵ�.
	static void SetRenderTexture(
		LPDIRECT3DTEXTURE9 normalTex,
		LPDIRECT3DTEXTURE9 depthTex);

	//ViewProjection �� �����Ѵ�
	static void SetCamera(cCamera* pCam);

	void InitShadowMap(int shadowMapSize, int distance);

	//�ش� �����ÿ� ���� ������ �� ��������
	void BeginShadowRender(cCamera* pViewCam = NULL);

	//�ش� �����ÿ� ���� ������ �� ��������
	void EndShdowRender();

	cCamera* GetLightCamera()
	{
		return &this->lightCamera;
	}

	cRenderTexture* GetShadowRenderTexture()
	{
		return this->pShadowMap;
	}


	void SetDistance(float shadowDistance);

	//���� ���� �Լ�
	void Render();

};

