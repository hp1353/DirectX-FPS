#include "stdafx.h"
#include "cEffect.h"


cEffect::cEffect()
	: m_vPosition(0, 0, 0)
	, m_nFrameCount(0)
	, m_nCurrentFrameX(0)
	, m_nCurrentFrameY(0)
	, m_nMaxFrameX(0)
	, m_nMaxFrameY(0)
	, m_isPlay(true)
{
}


cEffect::~cEffect()
{
}

void cEffect::Setup(char* fileName, float width, float height, int frameX, int frameY)
{
	m_sFileName = fileName;
	m_nMaxFrameX = frameX;
	m_nMaxFrameY = frameY;

	m_vecVertex.resize(frameX);

	for (int i = 0; i < frameX; i++)
	{
		m_vecVertex[i].resize(6);

		m_vecVertex[i][0].p = D3DXVECTOR3(-width / 2, -height / 2, 0);
		m_vecVertex[i][0].t = D3DXVECTOR2((1.0f / (float)frameX) * (i), 1.0f / (float)frameY);
		m_vecVertex[i][0].c = D3DCOLOR_ARGB(255, 255, 255, 255);
		
		m_vecVertex[i][1].p = D3DXVECTOR3(-width / 2, height / 2, 0);
		m_vecVertex[i][1].t = D3DXVECTOR2((1.0f / (float)frameX) * (i), 0);
		m_vecVertex[i][1].c = D3DCOLOR_ARGB(255, 255, 255, 255);
		 
		m_vecVertex[i][2].p = D3DXVECTOR3(width / 2, height / 2, 0);
		m_vecVertex[i][2].t = D3DXVECTOR2((1.0f / (float)frameX) * (i + 1), 0);
		m_vecVertex[i][2].c = D3DCOLOR_ARGB(255, 255, 255, 255);
		
		m_vecVertex[i][3].p = D3DXVECTOR3(-width / 2, -height / 2, 0);
		m_vecVertex[i][3].t = D3DXVECTOR2((1.0f / (float)frameX) * (i), 1.0f / (float)frameY);
		m_vecVertex[i][3].c = D3DCOLOR_ARGB(255, 255, 255, 255);
		
		m_vecVertex[i][4].p = D3DXVECTOR3(width / 2, height / 2, 0);
		m_vecVertex[i][4].t = D3DXVECTOR2((1.0f / (float)frameX) * (i + 1), 0);
		m_vecVertex[i][4].c = D3DCOLOR_ARGB(255, 255, 255, 255);
		
		m_vecVertex[i][5].p = D3DXVECTOR3(width / 2, -height / 2, 0);
		m_vecVertex[i][5].t = D3DXVECTOR2((1.0f / (float)frameX) * (i + 1), 1.0f / (float)frameY);
		m_vecVertex[i][5].c = D3DCOLOR_ARGB(255, 255, 255, 255);
	}

	SetRenderState();
}

void cEffect::Update()
{
	m_nFrameCount++;
	if (m_nFrameCount % 1 == 0)
	{
		m_nCurrentFrameX++;
		if (m_nCurrentFrameX >= m_nMaxFrameX)
		{
			m_nCurrentFrameX = 0;
			m_isPlay = false;
		}
		
		m_nFrameCount = 0;
	}
}

void cEffect::Render()
{
	if (!m_isPlay) return;

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PCT_VERTEX::FVF);	

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture(m_sFileName));

	SetBillboard(&matWorld);

	matWorld._41 = m_vPosition.x;
	matWorld._42 = m_vPosition.y;
	matWorld._43 = m_vPosition.z;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &(m_vecVertex[m_nCurrentFrameX][0]), sizeof(ST_PCT_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void cEffect::Destroy()
{
	this->Release();
}

void cEffect::SetRenderState()
{
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void cEffect::SetBillboard(D3DXMATRIXA16 * matWorld)
{
	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	*matWorld = matInvView;	
}
