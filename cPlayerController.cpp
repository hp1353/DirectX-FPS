#include "stdafx.h"
#include "cPlayerController.h"
#include "cHeightMap.h"
#include "cTextMap.h"
#include "cOBB.h"
#include "cBuilding.h"
#include "cBuildingGroup.h"
#include "cPlayer.h"
#include "cGun.h"

cPlayerController::cPlayerController()
	: m_pTarget(NULL)
{
}


cPlayerController::~cPlayerController()
{
}

void cPlayerController::Setup(float moveSpeed, cCharacter* pCharacter)
{
	m_fMoveSpeed = moveSpeed;

	m_pTarget = (cPlayer*)pCharacter;
}

void cPlayerController::Update(D3DXVECTOR3 & camAngle, OUT D3DXVECTOR3 & vDirection, OUT D3DXVECTOR3 & vPosition)
{
	if (m_pTarget == NULL) return;

	// angle을 이용해 direction을 구한다. 
	D3DXMATRIXA16 matR, matRX, matRY, matT;
	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);
	D3DXVECTOR3 mvDirection = D3DXVECTOR3(0, 0, 1);

	// 이동하기위한 Direction, 
	matR = matRY;
	D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);

	// 총알 발사하기위한 Direction Y값 필요
	vDirection = D3DXVECTOR3(0, 0, 1);
	matR = matRX * matRY;
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);

	if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
	{
		if(!m_pTarget->GetGun()->IsShoot())
			m_pTarget->BulletFire(vDirection);
	}

	if (g_pKeyManager->IsStayKeyDown(VK_RBUTTON))
		m_pTarget->SetGunMode(GUN_ZOOM_MODE);
	else
		m_pTarget->SetGunMode(GUN_NORMAL_MODE);
	
	D3DXVECTOR3 _vPosition = vPosition;

	if (m_pTarget->GetGunMode() == GUN_NORMAL_MODE)
	{
		// direction 방향으로 앞으로,
		if (g_pKeyManager->IsStayKeyDown('W'))			// 앞으로 움직임
		{
			if (!g_pSoundManager->isPlaySound("Walk"))
				g_pSoundManager->play("Walk", 0.5f * g_pSoundManager->GetSoundVol());

			_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
		}
		else if (g_pKeyManager->IsStayKeyDown('S'))		// 뒤로 움직임
		{
			if (!g_pSoundManager->isPlaySound("Walk"))
				g_pSoundManager->play("Walk", 0.5f * g_pSoundManager->GetSoundVol());

			_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
		}

		D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
		mvDirection = D3DXVECTOR3(0, 0, 1);
		D3DXVec3TransformNormal(&mvDirection, &mvDirection, &matR);

		if (g_pKeyManager->IsStayKeyDown('A'))			// 왼쪽으로 움직임
		{
			if (!g_pSoundManager->isPlaySound("Walk"))
				g_pSoundManager->play("Walk", 0.5f * g_pSoundManager->GetSoundVol());

			_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
		}
		else if (g_pKeyManager->IsStayKeyDown('D'))		// 오른쪽으로 움직임
		{
			if (!g_pSoundManager->isPlaySound("Walk"))
				g_pSoundManager->play("Walk", 0.5f * g_pSoundManager->GetSoundVol());

			_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
		}
	}
	else
	{
		if (g_pKeyManager->IsStayKeyDown('A'))			// 왼쪽으로 움직임
		{
			// 왼쪽으로 카메라 회전시킨다
		//	_vPosition = vPosition + (mvDirection * m_fMoveSpeed);
			//camAngle.z += 0.01f;
		}
		else if (g_pKeyManager->IsStayKeyDown('D'))		// 오른쪽으로 움직임
		{
			//camAngle.z -= 0.01f;
			// 오른쪽으로 카메라를 회전시킨다.
			//_vPosition = vPosition - (mvDirection * m_fMoveSpeed);
		}
	}


	if (m_pTarget->GetBuildings())
	{
		D3DXMATRIXA16 matWorld = *m_pOBB->GetWorldTM();
		D3DXVECTOR3 vDelta(_vPosition - vPosition);
		D3DXMATRIXA16 matT;
		
		D3DXMatrixTranslation(&matT, vDelta.x, vDelta.y, vDelta.z);
		matWorld = matWorld * matT;
		m_pOBB->Update(&matWorld);

		for (size_t i = 0; i < m_pTarget->GetBuildings()->GetBuilding().size(); i++)
		{
			if (cOBB::IsCollision(m_pTarget->GetBuildings()->GetBuilding()[i]->GetOBB(), m_pOBB))
				return;
		}
	}

	if (m_pHeightMap)
	{
		if (m_pHeightMap->GetHeight(_vPosition.x, _vPosition.y, _vPosition.z))
		{
			vPosition = _vPosition;

		}
	}

}
