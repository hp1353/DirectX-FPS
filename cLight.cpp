#include "stdafx.h"
#include "cLight.h"
#include "cShaderLoader.h"

LPD3DXEFFECT cLight::pLightEffect = NULL;		//라이트 Effect


cLight::cLight()
	: pShadowMap(NULL)
{
	if (pLightEffect == NULL) {
		cShaderLoader loader;
		pLightEffect = loader.LoadEffect("Shader/LightRender.fx");
		pLightEffect->SetFloat("halfPixelSizeX", (0.5f / WINSIZEX));
		pLightEffect->SetFloat("halfPixelSizeY", (0.5f / WINSIZEY));

	}

	lightColor = D3DXCOLOR(1, 1, 1, 1);
	intensity = 1.0f;

}


cLight::~cLight()
{
	SAFE_DELETE(pShadowMap);
	SAFE_RELEASE(pLightEffect);
}

void cLight::SetRenderTexture(LPDIRECT3DTEXTURE9 normalTex, LPDIRECT3DTEXTURE9 depthTex)
{
	if (pLightEffect == NULL) {
		cShaderLoader loader;
		pLightEffect = loader.LoadEffect("Shader/LightRender.fx");
		pLightEffect->SetFloat("halfPixelSizeX", (0.5f / WINSIZEX));
		pLightEffect->SetFloat("halfPixelSizeY", (0.5f / WINSIZEY));
	}

	//셰이더에 Texture 를 셋팅한다.
	pLightEffect->SetTexture("NormalTex", normalTex);
	pLightEffect->SetTexture("DepthTex", depthTex);
}

void cLight::SetCamera(cCamera * pCam)
{
	if (pLightEffect == NULL) {
		cShaderLoader loader;
		pLightEffect = loader.LoadEffect("Shader/LightRender.fx");
		pLightEffect->SetFloat("halfPixelSizeX", (0.5f / WINSIZEX));
		pLightEffect->SetFloat("halfPixelSizeY", (0.5f / WINSIZEY));
	}

	//ViewProjection 행렬을 구한다.
	D3DXMATRIXA16 matViewProjection = pCam->matViewProjection;

	//ViewProjection 역행렬
	D3DXMATRIXA16 matInvViewProjection;
	D3DXMatrixInverse(&matInvViewProjection, NULL, &matViewProjection);


	//셰이더에 카메라 행렬 셋팅
	pLightEffect->SetMatrix("matViewProjection", &matViewProjection);

	//셰이더에 카메라 역행렬 셋팅
	pLightEffect->SetMatrix("matInvViewProjection", &matInvViewProjection);


	//시점자의 위치 셋팅
	pLightEffect->SetVector("camPos", &D3DXVECTOR4(pCam->GetEye(), 1));

}

void cLight::InitShadowMap(int shadowMapSize, int distance)
{
	if (this->pShadowMap != NULL) {
		this->pShadowMap->Release();
		SAFE_DELETE(this->pShadowMap);
	}

	//쉐도우맵 준비 ( 깊이 값을 쓴다 )
	this->pShadowMap = new cRenderTexture();
	this->pShadowMap->Init(shadowMapSize, shadowMapSize, D3DFMT_R32F);

	//방향성 광원의 카메라는 직교로 쏜다. 종횡비는 1.0f;
	this->lightCamera.Ortho = true;
	this->lightCamera.AspectRatio = 1.0f;
	this->lightCamera.OrthoSize = distance;
	this->lightCamera.Far = distance * 2.0f;

	this->shadowDistance = distance;

}

void cLight::BeginShadowRender(cCamera * pViewCam)
{
	this->pShadowMap->BeginRender(0, 0xFFFFFFFF,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, TRUE);

	if (pViewCam != NULL)
	{
		//라이트 카메라를 최대한 메인카메라 앞에존재하게끔 위치를 설정한다.

		//뷰 카메라의 위치
		D3DXVECTOR3 camPos = pViewCam->GetEye();

		//뷰 카메라의 정면
		D3DXVECTOR3 camFront = pViewCam->GetDir();

		//광원의 방향
		D3DXVECTOR3 lightDir = Dir;// = this->Transform.GetForward();
		

		//카메라의 위치와 광원의 방향을따져 광원의 방향을유지한체 시점방향 앞에 라이트 투영영역이 유지되게 한다.
		D3DXVECTOR3 lightCamPos = camPos +
			(camFront * (shadowDistance * 0.5f)) +		//쉐도우 거리에 절반만큼 카메라 정면으로 이동
			(-lightDir * shadowDistance);				//라이트 반대 방향으로 쉐도우 거리 만큼이동,


		////라이트 카메라를 lightCamPos 위치에 있게
		//this->lightCamera.Transform.SetWorldPosition(lightCamPos);

		////방향을 라이트 방향과 같게.
		//this->lightCamera.Transform.LookDirection(lightDir);

		// direction셋팅
		D3DXVECTOR3 forward, right, up(0,1,0);
		D3DXVec3Normalize(&forward, &lightDir);

		//상방기준의 벡터와 정면을 외적하여 오른쪽벡터를 직교화한다.
		D3DXVec3Cross(&right, &up, &forward);
		D3DXVec3Normalize(&right, &right);

		//정면의 벡터와 우측벡터를 외적하여 상방벡터를 직교화한다.
		D3DXVec3Cross(&up, &forward, &right);
		D3DXVec3Normalize(&up, &up);

		// updateTransform
		D3DXVec3Normalize(&right, &right);
		D3DXVec3Normalize(&up, &up);
		D3DXVec3Normalize(&forward, &forward);

		//Final 행렬
		D3DXVECTOR3 r = right;// *this->scale.x;
		D3DXVECTOR3 u = up;// *this->scale.y;
		D3DXVECTOR3 f = forward;// *this->scale.z;
		D3DXVECTOR3 p = lightCamPos;

		// matLocal Matrix 에 적용
		D3DXMATRIXA16 matLocal;
		matLocal._11 = r.x;		matLocal._12 = r.y;		matLocal._13 = r.z;		matLocal._14 = 0;
		matLocal._21 = u.x;		matLocal._22 = u.y;		matLocal._23 = u.z;		matLocal._24 = 0;
		matLocal._31 = f.x;		matLocal._32 = f.y;		matLocal._33 = f.z;		matLocal._34 = 0;
		matLocal._41 = p.x;		matLocal._42 = p.y;		matLocal._43 = p.z;		matLocal._44 = 1;

		lightCamera.m_matWorld = matLocal;

	}// end if

	 //라이트 카메라에대한 행렬 업데이트
	this->lightCamera.UpdateLightCam();
}

void cLight::EndShdowRender()
{
	this->pShadowMap->EndRender();
}

void cLight::SetDistance(float shadowDistance)
{
	//방향성 광원의 카메라는 직교로 쏜다. 종횡비는 1.0f;
	this->lightCamera.Ortho = true;
	this->lightCamera.AspectRatio = 1.0f;
	this->lightCamera.OrthoSize = shadowDistance;
	this->lightCamera.Far = shadowDistance * 2.0f;

	this->shadowDistance = shadowDistance;
}

void cLight::Render()
{
	pLightEffect->SetTechnique("DirectionLight");




	//방항성광원 방향 셋팅
	pLightEffect->SetVector(
		"lightDir", &D3DXVECTOR4(this->Dir, 1));

	//컬러 셋팅
	pLightEffect->SetVector(
		"lightColor", (D3DXVECTOR4*)&this->lightColor);

	//라이트 강도 셋팅
	pLightEffect->SetFloat("intensity", this->intensity);


	//라이트 카메라의 ViewProjection
	pLightEffect->SetMatrix("matDirectionLightViewProj",
		&this->lightCamera.matViewProjection);

	//쉐도우 맵
	pLightEffect->SetTexture("ShadowTex", this->pShadowMap->GetTexture());

	UINT pass;
	pLightEffect->Begin(&pass, 0);
	for (UINT i = 0; i < pass; i++)
	{
		pLightEffect->BeginPass(i);
		directnalQuad.Render();
		pLightEffect->EndPass();
	}
	pLightEffect->End();

}
