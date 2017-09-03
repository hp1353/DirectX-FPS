#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cPlayerController.h"
#include "cRay.h"
#include "cOBB.h"
#include "cHeightMap.h"
#include "cTextMap.h"
#include "cSkinnedMesh.h"
#include "cRay.h"  
#include "cUIImageView.h"
#include "cUITextView.h"

cPlayer::cPlayer()
	: m_nSelectGun(0)
	, m_pPlayerOBB(NULL)
	, m_pBulletRay(NULL)
	, m_pUICursorRoot(NULL)
	, m_pUIGunInfoRoot(NULL)
	, m_pUIPlayerInfoRoot(NULL)
	, m_eGunMode(GUN_NORMAL_MODE)
	, m_fActionPower(0.f)
	, m_isAction(false)
	, m_isActionUp(false)
{
}


cPlayer::~cPlayer()
{
	SAFE_DELETE(m_pBulletRay);

	if (m_pUICursorRoot)
		m_pUICursorRoot->Destroy();

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Destroy();

	if (m_pUIGunInfoRoot)
		m_pUIGunInfoRoot->Destroy();

	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pController);
	SAFE_DELETE(m_pPlayerOBB);

	for each (auto p in m_vecGun)
		SAFE_RELEASE(p);

	m_pGun = NULL;
	m_vecGun.clear();
}

void cPlayer::Setup()
{
	m_vecGun.resize(GUNMAX);
	
	cGun* pGun1 = new cGun;
	pGun1->Setup(&m_vPosition, "Gun/shotgun/", "shotgun.X");
	pGun1->SetAttackPower(1.f);
	pGun1->SetMagazine(8.f);
	pGun1->SetCurrentBullet(8.f);
	m_vecGun[0] = pGun1;

	cGun* pGun2 = new cGun;
	pGun2->Setup(&m_vPosition, "Gun/winc/", "winchester.X");
	pGun2->SetAttackPower(1.f);
	pGun2->SetMagazine(10.f);
	pGun2->SetCurrentBullet(10.f);
	m_vecGun[1] = pGun2;
	
	cGun* pGun3 = new cGun;
	pGun3->Setup(&m_vPosition, "Gun/9mm/", "9mm.X");
	pGun3->SetAttackPower(1.f);
	pGun3->SetMagazine(7.f);
	pGun3->SetCurrentBullet(7.f);
	m_vecGun[2] = pGun3;

	m_pGun = m_vecGun[m_nSelectGun];

	m_pPlayerOBB = new cOBB;
	m_pPlayerOBB->Setup(D3DXVECTOR3(-0.5f, 0.0f, -0.5f),
		D3DXVECTOR3(0.5f, 2.0f, 0.5f));

	m_pController = new cPlayerController;
	m_pController->Setup(0.1f, this);
	m_pController->SetOBB(m_pPlayerOBB);

	m_stSphere.fRadius = PLAYER_BOUNDING_SPHERE_SIZE;
	m_stSphere.vCenter = m_vPosition;

	m_cFireGunSoundName = "ShotgunFire";
	m_cReloadGunSoundName = "ShotgunReload";

	SettingCursorUI();
	SettingGunUI();
	SettingPlayerInfoUI();
}

void cPlayer::Update(D3DXVECTOR3 & camAngle)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	if (m_pGun)
		m_pGun->Update();

	if (g_pKeyManager->IsOnceKeyDown('R'))
	{
		m_pGun->GetGunMesh()->PlayOneShot("reload", 0, 0);
		m_pGun->Reload();

		if (!g_pSoundManager->isPlaySound(m_cReloadGunSoundName))
			g_pSoundManager->play(m_cReloadGunSoundName, 0.7f * g_pSoundManager->GetSoundVol());
	}

	if (g_pKeyManager->IsOnceKeyDown('1'))
		GunChange();

	if (m_pPlayerOBB)
	{
		D3DXMATRIXA16 matOBBWorld, matT, matRX, matRY;
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
		D3DXMatrixRotationX(&matRX, camAngle.x);
		D3DXMatrixRotationY(&matRY, camAngle.y);

		matOBBWorld = matRX * matRY * matT;

		m_pPlayerOBB->Update(&matOBBWorld);
	}

	UIUpdate(camAngle.y);
	GunSetting(camAngle);

	m_stSphere.vCenter = m_vPosition;
	m_stSphere.vCenter.y = 1;

}

void cPlayer::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, false);
	
	if (m_pGun)
		m_pGun->Render();

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	
	UIRender();
}

void cPlayer::SetHeightMap(cHeightMap * hMap)
{
	m_pController->SetHeightMap(hMap);
}

void cPlayer::SetTextMap(cTextMap * tMap)
{
	m_pController->SetTextMap(tMap);
}

void cPlayer::GunChange()
{
	m_nSelectGun++;
	m_nSelectGun %= m_vecGun.size();
	m_pGun = m_vecGun[m_nSelectGun];

	switch (m_nSelectGun)
	{
	case 0:
		m_pImageGunFront->SetTexture("PlayerUI/shotgun_front.png");
		m_pImageGunBack->SetTexture("PlayerUI/shotgun_back.png");
		m_cFireGunSoundName = "ShotgunFire";
		m_cReloadGunSoundName = "ShotgunReload";
		break;
	case 1:
		m_pImageGunFront->SetTexture("PlayerUI/winc_front.png");
		m_pImageGunBack->SetTexture("PlayerUI/winc_back.png");
		m_cFireGunSoundName = "WincFire";
		m_cReloadGunSoundName = "WincReload";
		break;
	case 2:
		m_pImageGunFront->SetTexture("PlayerUI/9mm_front.png");
		m_pImageGunBack->SetTexture("PlayerUI/9mm_back.png");
		m_cFireGunSoundName = "9mmFire";
		m_cReloadGunSoundName = "9mmReload";
		break;
	}

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pImageGunFront->SetPosition(rc.right - m_pImageGunFront->GetSize().nWidth / 2 - 90, rc.bottom - m_pImageGunFront->GetSize().nHeight / 2 - 70);
	m_pImageGunBack->SetPosition(rc.right - m_pImageGunBack->GetSize().nWidth / 2 - 90, rc.bottom - m_pImageGunBack->GetSize().nHeight / 2 - 70);

}

void cPlayer::GunSetting(D3DXVECTOR3 & camAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matRZ, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationY(&matR, -D3DX_PI / 2);

	float moveX;

	if (m_eGunMode == GUN_NORMAL_MODE)
		moveX = 1.0f;
	else
		moveX = 0.f;


	D3DXMatrixTranslation(&matT, m_vPosition.x + moveX, m_vPosition.y + 0.5f, m_vPosition.z + 3.0f);

	// 중심 축 맞춰주기 위해 이동후 회전, 다시 원위치로
	D3DXMATRIXA16 matTempT, matTempTInv;
	D3DXMatrixTranslation(&matTempT, moveX, 0.5, 3.0f);
	D3DXMatrixTranslation(&matTempTInv, -moveX, -0.5, -3.0f);

	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);

	matR = matR * matTempT * matRX * matRY * matTempTInv;


	matSRT = matS * matR * matT;

	m_matWorldTM = matSRT;

	m_pGun->SetWorldMatrix(&m_matWorldTM);
}

void cPlayer::BulletFire(D3DXVECTOR3 dir)
{
	if (m_pGun)
	{
		if (m_pGun->IsReload() || m_pGun->IsShoot())
			return;

		if (m_pGun->GetCurrentBullet() <= 0)
		{
			m_pGun->GetGunMesh()->PlayOneShot("reload", 0, 0);
			m_pGun->Reload();

			if (!g_pSoundManager->isPlaySound(m_cReloadGunSoundName))
			{
				g_pSoundManager->play(m_cReloadGunSoundName, 0.7f * g_pSoundManager->GetSoundVol());
			}
			return;
		}
		if (!m_pGun->Fire(dir, m_matWorldTM))
			return;

		m_pGun->GetGunMesh()->PlayOneShot("shot", 0, 0);
		g_pSoundManager->play(m_cFireGunSoundName, 0.7f * g_pSoundManager->GetSoundVol());

		m_pBulletRay = new cRay;

		m_isAction = true;
		m_isActionUp = true;
	}
}

cRay * cPlayer::GetBullet()
{
	return m_pBulletRay;
}

void cPlayer::DeleteRay()
{
	SAFE_DELETE(m_pBulletRay);
}

void cPlayer::SettingCursorUI()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pUICursorRoot = new cUIObject;
	m_pUICursorRoot->SetPosition(0, 0);

	m_pImageCursorL = new cUIImageView;
	m_pImageCursorL->SetTexture("PlayerUI/cursor_h.tga");
	m_pImageCursorL->SetPosition(CENTERX - 25, CENTERY + 10);

	m_pImageCursorR = new cUIImageView;
	m_pImageCursorR->SetTexture("PlayerUI/cursor_h.tga");
	m_pImageCursorR->SetPosition(CENTERX + 35, CENTERY + 10);

	m_pImageCursorT = new cUIImageView;
	m_pImageCursorT->SetTexture("PlayerUI/cursor_v.tga");
	m_pImageCursorT->SetPosition(CENTERX + 7, CENTERY - 20);

	m_pImageCursorB = new cUIImageView;
	m_pImageCursorB->SetTexture("PlayerUI/cursor_v.tga");
	m_pImageCursorB->SetPosition(CENTERX + 7, CENTERY + 40);

	m_pUICursorRoot->AddChild(m_pImageCursorL);
	m_pUICursorRoot->AddChild(m_pImageCursorR);
	m_pUICursorRoot->AddChild(m_pImageCursorT);
	m_pUICursorRoot->AddChild(m_pImageCursorB);
}

void cPlayer::SettingGunUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pUIGunInfoRoot = new cUIObject;
	m_pUIGunInfoRoot->SetPosition(0, 0);

	cUIImageView* pAmmoBackground = new cUIImageView;
	pAmmoBackground->SetTexture("PlayerUI/Ammo_background.png");
	pAmmoBackground->SetPosition(rc.right - pAmmoBackground->GetSize().nWidth / 2 - 3, rc.bottom - pAmmoBackground->GetSize().nHeight / 2 - 3);
	
	m_pImageGunFront = new cUIImageView;
	m_pImageGunFront->SetTexture("PlayerUI/shotgun_front.png");
	m_pImageGunFront->SetPosition(rc.right - m_pImageGunFront->GetSize().nWidth / 2 - 90, rc.bottom - m_pImageGunFront->GetSize().nHeight / 2 - 70);

	m_pImageGunBack = new cUIImageView;
	m_pImageGunBack->SetTexture("PlayerUI/shotgun_back.png");
	m_pImageGunBack->SetPosition(rc.right - m_pImageGunBack->GetSize().nWidth / 2 - 90, rc.bottom - m_pImageGunBack->GetSize().nHeight / 2 - 70);
	
	m_pUIGunInfoRoot->AddChild(pAmmoBackground);
	m_pUIGunInfoRoot->AddChild(m_pImageGunFront);
	m_pUIGunInfoRoot->AddChild(m_pImageGunBack);

	// 텍스트 UI
	m_pBulletText = new cUITextView;
	m_pBulletText->SetText("0/0");
	m_pBulletText->SetSize(ST_SIZEN(200, 100));
	m_pBulletText->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	m_pBulletText->SetPosition(pAmmoBackground->GetPosition().x + 50, pAmmoBackground->GetPosition().y - 10);

	m_pUIGunInfoRoot->AddChild(m_pBulletText);

}

void cPlayer::SettingPlayerInfoUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pUIPlayerInfoRoot = new cUIObject;
	m_pUIPlayerInfoRoot->SetPosition(0, 0);

	cUIImageView* pHpBack = new cUIImageView;
	pHpBack->SetTexture("PlayerUI/HpBack.png");
	pHpBack->SetPosition(rc.left + pHpBack->GetSize().nWidth / 2 + 20, rc.bottom - pHpBack->GetSize().nHeight / 2);

	cUIImageView* pCompassBack = new cUIImageView;
	pCompassBack->SetTexture("PlayerUI/compass_back.png");
	pCompassBack->SetPosition(rc.left + pCompassBack->GetSize().nWidth / 2 + 60, rc.bottom - pHpBack->GetSize().nHeight - pCompassBack->GetSize().nHeight / 2 - 10);

	m_pUIPlayerInfoRoot->AddChild(pHpBack);
	m_pUIPlayerInfoRoot->AddChild(pCompassBack);

	// 움직이는 UI
	m_pCompassFront = new cUIImageView;
	m_pCompassFront->SetTexture("PlayerUI/compass_front.png");
	m_pCompassFront->SetPosition(pCompassBack->GetPosition().x, pCompassBack->GetPosition().y);

	m_pUIPlayerHP = new cUIImageView;
	m_pUIPlayerHP->SetTexture("PlayerUI/hp_bar_front.png");
	m_pUIPlayerHP->SetPosition(pHpBack->GetPosition().x + 31, pHpBack->GetPosition().y + 6);

	m_pUIPlayerInfoRoot->AddChild(m_pCompassFront);
	m_pUIPlayerInfoRoot->AddChild(m_pUIPlayerHP);
}

void cPlayer::CursorAction(float ActionPower)
{
	if (m_fActionPower > ActionPower)
		m_isActionUp = false;

	if (m_isActionUp)
	{
		m_fActionPower += 5.0f;
	}	
	else
	{
		m_fActionPower -= 1.0f;
		if (m_fActionPower <= 0)
		{
			m_fActionPower = 0.f;
			m_isAction = false;
		}
	}

	m_pImageCursorL->SetPosition(CENTERX - 25 - m_fActionPower, CENTERY + 10); //left
	m_pImageCursorR->SetPosition(CENTERX + 35 + m_fActionPower, CENTERY + 10); //right
	m_pImageCursorT->SetPosition(CENTERX + 7, CENTERY - 20 - m_fActionPower); // top
	m_pImageCursorB->SetPosition(CENTERX + 7, CENTERY + 40 + m_fActionPower); // bottom

}

void cPlayer::UIUpdate(float angle)
{
	if (m_isAction)
		CursorAction(30.f);

	if (m_pUICursorRoot)
		m_pUICursorRoot->Update();

	if (m_pImageGunBack)
		m_pImageGunBack->SetRateY(1 - m_pGun->GetCurrentExp() / m_pGun->GetMaxExp());

	if (m_pBulletText)
		m_pBulletText->SetText(std::to_string(m_pGun->GetCurrentBullet())
			+ "/" + std::to_string(m_pGun->GetMaxBullet()));

	if (m_pUIGunInfoRoot)
		m_pUIGunInfoRoot->Update();

	if (m_pCompassFront)
		m_pCompassFront->SetAngle(angle);

	if (m_pUIPlayerHP)
		m_pUIPlayerHP->SetRateX(m_fCurrentHp / m_fMaxHp);

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Update();
}

void cPlayer::UIRender()
{
	if (m_pUICursorRoot)
		m_pUICursorRoot->Render(m_pSprite);
	
	if (m_pUIGunInfoRoot)
		m_pUIGunInfoRoot->Render(m_pSprite);

	if (m_pUIPlayerInfoRoot)
		m_pUIPlayerInfoRoot->Render(m_pSprite);
}

D3DXVECTOR3 cPlayer::GetSphereCenter()
{
	m_stSphere.vCenter = m_vPosition;
	m_stSphere.vCenter.y = 1;

	return m_stSphere.vCenter;
}
