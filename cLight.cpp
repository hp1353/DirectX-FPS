#include "stdafx.h"
#include "cLight.h"
#include "cShaderLoader.h"

LPD3DXEFFECT cLight::pLightEffect = NULL;		//����Ʈ Effect


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

	//���̴��� Texture �� �����Ѵ�.
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

	//ViewProjection ����� ���Ѵ�.
	D3DXMATRIXA16 matViewProjection = pCam->matViewProjection;

	//ViewProjection �����
	D3DXMATRIXA16 matInvViewProjection;
	D3DXMatrixInverse(&matInvViewProjection, NULL, &matViewProjection);


	//���̴��� ī�޶� ��� ����
	pLightEffect->SetMatrix("matViewProjection", &matViewProjection);

	//���̴��� ī�޶� ����� ����
	pLightEffect->SetMatrix("matInvViewProjection", &matInvViewProjection);


	//�������� ��ġ ����
	pLightEffect->SetVector("camPos", &D3DXVECTOR4(pCam->GetEye(), 1));

}

void cLight::InitShadowMap(int shadowMapSize, int distance)
{
	if (this->pShadowMap != NULL) {
		this->pShadowMap->Release();
		SAFE_DELETE(this->pShadowMap);
	}

	//������� �غ� ( ���� ���� ���� )
	this->pShadowMap = new cRenderTexture();
	this->pShadowMap->Init(shadowMapSize, shadowMapSize, D3DFMT_R32F);

	//���⼺ ������ ī�޶�� ������ ���. ��Ⱦ��� 1.0f;
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
		//����Ʈ ī�޶� �ִ��� ����ī�޶� �տ������ϰԲ� ��ġ�� �����Ѵ�.

		//�� ī�޶��� ��ġ
		D3DXVECTOR3 camPos = pViewCam->GetEye();

		//�� ī�޶��� ����
		D3DXVECTOR3 camFront = pViewCam->GetDir();

		//������ ����
		D3DXVECTOR3 lightDir = Dir;// = this->Transform.GetForward();
		

		//ī�޶��� ��ġ�� ������ ���������� ������ ������������ü �������� �տ� ����Ʈ ���������� �����ǰ� �Ѵ�.
		D3DXVECTOR3 lightCamPos = camPos +
			(camFront * (shadowDistance * 0.5f)) +		//������ �Ÿ��� ���ݸ�ŭ ī�޶� �������� �̵�
			(-lightDir * shadowDistance);				//����Ʈ �ݴ� �������� ������ �Ÿ� ��ŭ�̵�,


		////����Ʈ ī�޶� lightCamPos ��ġ�� �ְ�
		//this->lightCamera.Transform.SetWorldPosition(lightCamPos);

		////������ ����Ʈ ����� ����.
		//this->lightCamera.Transform.LookDirection(lightDir);

		// direction����
		D3DXVECTOR3 forward, right, up(0,1,0);
		D3DXVec3Normalize(&forward, &lightDir);

		//�������� ���Ϳ� ������ �����Ͽ� �����ʺ��͸� ����ȭ�Ѵ�.
		D3DXVec3Cross(&right, &up, &forward);
		D3DXVec3Normalize(&right, &right);

		//������ ���Ϳ� �������͸� �����Ͽ� ��溤�͸� ����ȭ�Ѵ�.
		D3DXVec3Cross(&up, &forward, &right);
		D3DXVec3Normalize(&up, &up);

		// updateTransform
		D3DXVec3Normalize(&right, &right);
		D3DXVec3Normalize(&up, &up);
		D3DXVec3Normalize(&forward, &forward);

		//Final ���
		D3DXVECTOR3 r = right;// *this->scale.x;
		D3DXVECTOR3 u = up;// *this->scale.y;
		D3DXVECTOR3 f = forward;// *this->scale.z;
		D3DXVECTOR3 p = lightCamPos;

		// matLocal Matrix �� ����
		D3DXMATRIXA16 matLocal;
		matLocal._11 = r.x;		matLocal._12 = r.y;		matLocal._13 = r.z;		matLocal._14 = 0;
		matLocal._21 = u.x;		matLocal._22 = u.y;		matLocal._23 = u.z;		matLocal._24 = 0;
		matLocal._31 = f.x;		matLocal._32 = f.y;		matLocal._33 = f.z;		matLocal._34 = 0;
		matLocal._41 = p.x;		matLocal._42 = p.y;		matLocal._43 = p.z;		matLocal._44 = 1;

		lightCamera.m_matWorld = matLocal;

	}// end if

	 //����Ʈ ī�޶󿡴��� ��� ������Ʈ
	this->lightCamera.UpdateLightCam();
}

void cLight::EndShdowRender()
{
	this->pShadowMap->EndRender();
}

void cLight::SetDistance(float shadowDistance)
{
	//���⼺ ������ ī�޶�� ������ ���. ��Ⱦ��� 1.0f;
	this->lightCamera.Ortho = true;
	this->lightCamera.AspectRatio = 1.0f;
	this->lightCamera.OrthoSize = shadowDistance;
	this->lightCamera.Far = shadowDistance * 2.0f;

	this->shadowDistance = shadowDistance;
}

void cLight::Render()
{
	pLightEffect->SetTechnique("DirectionLight");




	//���׼����� ���� ����
	pLightEffect->SetVector(
		"lightDir", &D3DXVECTOR4(this->Dir, 1));

	//�÷� ����
	pLightEffect->SetVector(
		"lightColor", (D3DXVECTOR4*)&this->lightColor);

	//����Ʈ ���� ����
	pLightEffect->SetFloat("intensity", this->intensity);


	//����Ʈ ī�޶��� ViewProjection
	pLightEffect->SetMatrix("matDirectionLightViewProj",
		&this->lightCamera.matViewProjection);

	//������ ��
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
