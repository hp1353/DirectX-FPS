#include "stdafx.h"
#include "cEvent.h"
#include "cUIImageView.h"
#include "cUIObject.h"


cEvent::cEvent()
	: m_nCount(0)
	, m_pSprite(NULL)
	, m_isPlay(true)
	, m_nTexturNum(0)
	, m_fAlpha(15.f)
	, m_fDeltaAlpha(2.0f)
	, m_fAngle(0.f)
{
}


cEvent::~cEvent()
{
	SAFE_RELEASE(m_pSprite);
}

void cEvent::Setup(char * FirstszFullPath, char * SecondszFullPath, int fadeInSpeed, int fadeOutSpeed)
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	D3DXIMAGE_INFO stImageInfo;
	m_aTexture[0] = g_pTextureManager->GetTexture(FirstszFullPath, &stImageInfo);
	m_nWidth[0] = stImageInfo.Width;
	m_nHeight[0] = stImageInfo.Height;

	m_aTexture[1] = g_pTextureManager->GetTexture(SecondszFullPath, &stImageInfo);
	m_nWidth[1] = stImageInfo.Width;
	m_nHeight[1] = stImageInfo.Height;

	m_nFadeInSpeed = fadeInSpeed;
	m_nFadeOutSpeed = fadeOutSpeed;

	m_fDeltaAlpha = 240.f / ((float)(m_nFadeInSpeed + m_nFadeOutSpeed) / 2.f);
}

void cEvent::Update()
{
	m_nCount++;
	
	if (m_nCount < m_nFadeInSpeed)
	{
		m_fAlpha += m_fDeltaAlpha;
		m_nTexturNum = 0;
	}
	else if (m_nCount > m_nFadeInSpeed && m_nCount < m_nFadeOutSpeed + m_nFadeInSpeed)
	{
		m_fAlpha -= m_fDeltaAlpha;
		m_nTexturNum = 1;
	}
	else if (m_nCount > m_nFadeOutSpeed + m_nFadeInSpeed)
	{
		m_isPlay = false;
	}
}

void cEvent::Render()
{
	D3DXMATRIXA16 m_matWorld, matT;
	//D3DXMatrixIdentity(&m_matWorld);
//	m_fAngle += 0.01f;
	D3DXMatrixRotationZ(&m_matWorld, m_fAngle);
	D3DXMatrixTranslation(&matT, CENTERX, CENTERY/* - 150*/, 0);
	m_matWorld = m_matWorld * matT;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pSprite->SetTransform(&m_matWorld);

	RECT rc;
	SetRect(&rc, 
		0,
		0,
		m_nWidth[m_nTexturNum],
		m_nHeight[m_nTexturNum]);

	m_pSprite->Draw(m_aTexture[m_nTexturNum],
		&rc,
		&D3DXVECTOR3(m_nWidth[m_nTexturNum] / 2, m_nHeight[m_nTexturNum] / 2 + 150, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB((int)m_fAlpha, 255, 255, 255));

	m_pSprite->End();

}
