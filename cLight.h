#pragma once

#include "cTexQuad.h"
#include "cCamera.h"
#include "cRenderTexture.h"

class cLight
{
public:
	static LPD3DXEFFECT pLightEffect;		//라이트 Effect

public:
	D3DXMATRIXA16 Transform;		//광원의 Transform
	D3DXCOLOR lightColor;		//광원 색상
	float intensity;			//광원의 강도
	D3DXVECTOR3 Dir;

private:
	cTexQuad		directnalQuad;		//방향성 광원을 그리기 위한 스크린쿼드

	cCamera			lightCamera;		//방향성 광원의 그림자 맵을 그리기 위한 카메라...
	cRenderTexture*	pShadowMap;			//깊이값을 저장할 쉐도우 맵

	float			shadowDistance;

public:
	cLight();
	~cLight();

	//Texture 를 셋팅한다.
	static void SetRenderTexture(
		LPDIRECT3DTEXTURE9 normalTex,
		LPDIRECT3DTEXTURE9 depthTex);

	//ViewProjection 을 셋팅한다
	static void SetCamera(cCamera* pCam);

	void InitShadowMap(int shadowMapSize, int distance);

	//해당 라이팅에 대한 쉐도우 맵 랜더시작
	void BeginShadowRender(cCamera* pViewCam = NULL);

	//해당 라이팅에 대한 쉐도우 맵 랜더종료
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

	//광원 랜더 함수
	void Render();

};

