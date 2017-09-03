#include "stdafx.h"
#include "cAI.h"
#include "cAiController.h"
#include "cGun.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"
#include "cUIImageView.h"
#include "cTextMap.h"

cAI::cAI()
	: m_pAIOBB(NULL)
	, m_pAIPointPos(NULL)
	, m_isShow(false)
{
}


cAI::~cAI()
{
	SAFE_DELETE(m_pAIOBB);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pAIPointPos);
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pGun);
	SAFE_DELETE(m_pSkinnedMesh);
}

void cAI::Setup(char* szFolder, char* szFilename)
{
	// SkinnedMesh
	m_pSkinnedMesh = new cSkinnedMesh(szFolder, szFilename);
	
	// obb
	m_pAIOBB = new cOBB;
	m_pAIOBB->Setup(m_pSkinnedMesh, 0.01f);

	// position point 
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pAIPointPos = new cUIImageView;
	m_pAIPointPos->SetTexture("PlayerUI/enemy_point.png");
	m_pAIPointPos->SetPosition(100, 100);
	
	// GUN
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/MG_42/", "MG_42.X");
	m_pGun->SetCurrentBullet(10000);
	m_pGun->SetMaxBullet(10000);
	m_pGun->SetParentWorldMatrix(m_pSkinnedMesh->getMatrix("RightHandIndex1"));

	D3DXMATRIXA16 matRX, matRY, matR;
	D3DXMatrixRotationX(&matRX, D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, -D3DX_PI / 2 - 0.3f);

	matR = matRX * matRY;
	m_pGun->SetWorldMatrixByBoneName(&matR, "stock");

	// Controller
	m_pController = new cAIController;
	m_pController->Setup(0.1f, this);
	m_pController->SetOBB(m_pAIOBB);
	
	// bounding Sphere
	m_stBoundingSphere.fRadius = AI_BOUNDING_SPHERE_SIZE;

	m_vecBoundingSphereDetail.resize(11);
}

void cAI::Update(D3DXVECTOR3 vPlayer, float fAngle)
{
	UpdateSkinnedMesh(m_vDirection);
	cGameObject::Update();

	if (m_pController)
		m_pController->Update(vPlayer, m_vDirection, m_vPosition);
	
	if (m_pGun)
		m_pGun->Update();

	if (m_pAIPointPos)
	{
		D3DXVECTOR3 vPos((-vPlayer.x + m_vPosition.x), 0, (vPlayer.z - m_vPosition.z));
		
		if (vPos.x * vPos.x + vPos.z * vPos.z < 15 * 15)
		{
			m_isShow = true;

			// angle을 이용해서 회전 행렬을 만든다. 
			D3DXMATRIXA16 matR;
			D3DXMatrixRotationY(&matR, fAngle);

			// 행렬에 값을 곱해서 계산한다. 
			D3DXVec3TransformCoord(&vPos, &vPos, &matR);

			m_pAIPointPos->SetPosition(172 + vPos.x * 5,
				562 + vPos.z * 5, 0);
		}
		else 
			m_isShow = false;
	}

}

void cAI::Render()
{
	if (m_pSkinnedMesh)
		m_pSkinnedMesh->UpdateAndRender();

	SetBoundingSphere();

	if (m_pGun)
		m_pGun->Render();
}

void cAI::SpriteRender()
{
	if (m_isShow)
		if (m_pAIPointPos)
			m_pAIPointPos->Render(m_pSprite);
}

void cAI::SetHeightMap(cHeightMap * hMap)
{
	m_pController->SetHeightMap(hMap);
}

void cAI::SetTextMap(cTextMap * tMap)
{
	m_pController->SetTextMap(tMap);
}

void cAI::SetBoundingSphere()
{
	D3DXMATRIXA16 matSphere, matT;
	ST_SPHERE s;
	s.fRadius = AI_BOUNDING_SPHERE_DETAIL_SIZE;
	D3DXMatrixTranslation(&matT, 0, AI_BOUNDING_SPHERE_SIZE, 0);
	
	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("Jaw") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[0] = s;
	
	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("Spine2") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[1] = s;
	
	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("Hips") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[2] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftUpLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[3] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[4] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftFoot") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[5] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightUpLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[6] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightLeg") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[7] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightFoot") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[8] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("RightForeArm") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[9] = s;

	s.vCenter = D3DXVECTOR3(0, 0, 0);
	matSphere = *m_pSkinnedMesh->getLocalMatrix("LeftForeArm") * m_matWorldTM;
	D3DXVec3TransformCoord(&s.vCenter, &s.vCenter, &matSphere);
	m_vecBoundingSphereDetail[10] = s;

}

ST_SPHERE cAI::GetBoundingSphere()
{
	return m_stBoundingSphere;
}

std::vector<ST_SPHERE> cAI::GetBoundingSphereDetail()
{
	return m_vecBoundingSphereDetail;
}

void cAI::BulletFire(D3DXVECTOR3 dir)
{
	if (m_pGun)
	{
		if (m_pSkinnedMesh->IsPlay("StandFire"))
			return;

		m_pSkinnedMesh->PlayOneShot("StandFire", 0.3f, 0.3f);

		D3DXMATRIXA16 matT;
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

		m_pGun->Fire(dir, matT);
	}
}

void cAI::Destroy()
{
	m_pSkinnedMesh->PlayOneShotAfterHold(8);
	g_pSoundManager->play("AIDeath", 0.4 * g_pSoundManager->GetSoundVol());
}

bool cAI::IsDeath()
{
	return !m_pSkinnedMesh->IsPlay();
}

void cAI::UpdateSkinnedMesh(D3DXVECTOR3 &vDir)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matRDir, matT;
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matRX, -D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	D3DXMatrixLookAtLH(&matRDir,
		&D3DXVECTOR3(0, 0, 0),
		&vDir,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matRDir, &matRDir);
	
	matR = matRX * matRY * matRDir;
	m_matWorldTM = matS * matR * matT;

	m_pSkinnedMesh->SetTransform(&m_matWorldTM);

	D3DXMATRIXA16 obbWorld;
	obbWorld = matR * matT;
	if (m_pAIOBB)
		m_pAIOBB->Update(&obbWorld);

	m_stBoundingSphere.vCenter = m_vPosition;
	m_stBoundingSphere.vCenter.y += AI_BOUNDING_SPHERE_SIZE;
}
