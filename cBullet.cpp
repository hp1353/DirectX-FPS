#include "stdafx.h"
#include "cBullet.h"

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

cBullet::cBullet()
	: m_fMoveDistance(0.0f)
	, m_fBulletSpeed(1.0f)
	, m_pBoundingSphereMesh(NULL)
{
}


cBullet::~cBullet()
{
	SAFE_RELEASE(m_pBoundingSphereMesh);
}

void cBullet::Setup(D3DXVECTOR3 vDirection, D3DXVECTOR3 vPosition)
{
	m_vDirection = vDirection;
	m_vPosition = vPosition;
	m_fMoveDistance = 0.f;

	Setup_Particle();
	
	if (m_pBoundingSphereMesh == NULL)
	{
		D3DXCreateSphere(g_pD3DDevice,
			0.1f,
			20,
			20,
			&m_pBoundingSphereMesh,
			NULL);
	}

}

void cBullet::Update()
{
	Update_Particle(); 	/// >> : Particle Update
	m_fMoveDistance += m_fBulletSpeed;
	
}

void cBullet::Render()
{
	if(m_fMoveDistance < 1)
		Render_Particle(); /// : particle

	/*if (m_pBoundingSphereMesh)
	{
		D3DXMATRIXA16 matT;
		D3DXMatrixTranslation(&matT,
			m_stBoundingSphere.vCenter.x,
			m_stBoundingSphere.vCenter.y,
			m_stBoundingSphere.vCenter.z);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pBoundingSphereMesh->DrawSubset(0);
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}*/
}

void cBullet::Setup_Particle()
{
	/// >> : 파티클 랜덤하게 생성
	m_vecVertex.resize(100);
	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		float fRadius = 0.015f * i;// rand() % 10 / 10.0f;

		// 총알의 방향으로 늘어나게 보이는 효과
		m_vecVertex[i].p = m_vDirection * fRadius;

		D3DXMATRIX matT, matWorld;
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
		matWorld = matT;

		D3DXVec3TransformCoord(
			&m_vecVertex[i].p,
			&m_vecVertex[i].p,
			&matWorld);

		m_vecVertex[i].c = D3DCOLOR_ARGB(255, 180, 70, 20);
	}


	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);	/// >> : 포인트 확대 여부 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.f));	/// >> : 포인트 사이즈 설정 

																/// : 기본 도형에 대한 거리 사이즈의 감소를 제어 , 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));	/// : vs 10.0f check 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true); /// : 포인트에 텍스쳐를 입힐 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));	/// : 포인트 최소 크기
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(10.0f));	/// : 포인트 최대 크기 vs 10.0f check 

																		// 텍스쳐 알파 옵션 설정
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정.
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void cBullet::Update_Particle()
{
	if (m_vecVertex.empty()) return;

	m_stBoundingSphere.vCenter = m_vecVertex.back().p;
	m_stBoundingSphere.fRadius = 0.1f;

	for (size_t i = 0; i < m_vecVertex.size(); ++i)
	{
		m_vecVertex[i].p = m_vecVertex[i].p 
			+ D3DXVECTOR3(m_fBulletSpeed * m_vDirection.x, 
				m_fBulletSpeed * m_vDirection.y,
				m_fBulletSpeed * m_vDirection.z);
	}
}

void cBullet::Render_Particle()
{
	if (m_vecVertex.empty()) return;
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture("alpha_tex.tga"));
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
		m_vecVertex.size(),
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}
