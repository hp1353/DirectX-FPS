#include "stdafx.h"
#include "cLoadingScene.h"
#include "cUIImageView.h"


cLoadingScene::cLoadingScene()
	: m_pMainUIRoot(NULL)
	, m_pSprite(NULL)
{
}


cLoadingScene::~cLoadingScene()
{
	
}

void cLoadingScene::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pMainUIRoot = new cUIObject;
	m_pMainUIRoot->SetPosition(0, 0);

	cUIImageView* MainMenuBackground = new cUIImageView;
	MainMenuBackground->SetTexture("Loading/Loading1.png");
	MainMenuBackground->SetPosition(MainMenuBackground->GetSize().nWidth / 2, MainMenuBackground->GetSize().nHeight / 2);

	m_pMainUIRoot->AddChild(MainMenuBackground);

}

void cLoadingScene::Destroy()
{
	if (m_pMainUIRoot)
		m_pMainUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);

}

void cLoadingScene::Update()
{
	if (m_pMainUIRoot)
		m_pMainUIRoot->Update();
}

void cLoadingScene::Render()
{
	if (m_pMainUIRoot)
		m_pMainUIRoot->Render(m_pSprite);

	char szTemp[1024];
	sprintf(szTemp, "LOADING : %f", g_pSceneManager->fLoadingProgress);

	LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_DEFAULT);
	RECT rc;
	SetRect(&rc, 0, 40, 300, 300);
	pFont->DrawText(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 0));
}
