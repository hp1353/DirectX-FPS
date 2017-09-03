#include "stdafx.h"
#include "cAIController.h"
#include "cTextMap.h"
#include "cHeightMap.h"
#include "cBuilding.h"
#include "cOBB.h"
#include "cAI.h"

#include "cGun.h"

#include "cState.h"
#include "cStateMove.h"
#include "cStateAttack.h"
#include "cSkinnedMesh.h"


cAIController::cAIController()
	: m_fAttackRange(20.0f)
	, m_fDeltaTime(0.f)
	, m_pTarget(NULL)
{
}


cAIController::~cAIController()
{
}

void cAIController::Setup(float moveSpeed, cCharacter* pCharacter)
{
	m_fMoveSpeed = moveSpeed;
	m_fDeltaTime = 0.f;

	m_pTarget = (cAI*)pCharacter;
	
	if (m_pTarget)
	{
		cStateMove* pStateMove = new cStateMove;
		pStateMove->SetFrom(m_pTarget->GetPosition());
		pStateMove->SetTarget(m_pTarget);
		pStateMove->SetBuildings(m_pTarget->GetBuildings());
		pStateMove->Start();
		pStateMove->SetDelegate(pStateMove);

		m_pTarget->SetState(pStateMove);
		m_pTarget->GetMesh()->Play("Walk", 0.3f);
		SAFE_RELEASE(pStateMove);

		m_fAttackRange = m_pTarget->GetGun()->GetAttackRange();
	}
}

void cAIController::Update(OUT D3DXVECTOR3 & vPlayer, OUT D3DXVECTOR3 & vDirection, OUT D3DXVECTOR3 & vPosition)
{
	if (m_pTarget == NULL)
		return;

	m_fDeltaTime += g_pTimeManager->GetElapsedTime();
	/*
	
	
	if (m_fDeltaTime < 2.f)
		return;
	
	m_fDeltaTime = 0.f;
*/
	
	D3DXVECTOR3 vTargetPos = m_pTarget->GetPosition();
	float fPlayerDist = D3DXVec3Length(&(vPlayer - vTargetPos));
	
	if (fPlayerDist < m_fAttackRange)	// player�� Ž�� �Ÿ� �ȿ� ����
	{
		D3DXVECTOR3 vPlayerDir;	// ai���� player�� ������ ���� ����
		vPlayerDir = vPlayer - vTargetPos;
		D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);

		if (D3DXVec3Dot(&vDirection, &vPlayerDir) > 0.5f)	// player�� �þ߰� �ȿ� ����
		{
			if (fPlayerDist < m_fAttackRange - 1.5f)	// player�� ���� �Ÿ� �ȿ� ����
			{
				if (m_pTarget->GetState()->GetStateType() == STATE_ATTACK)
					return;
				if (m_pTarget->GetMesh()->IsPlay("StandFire"))
					return;
				
				m_fDeltaTime = 0.f;

				m_pTarget->GetMesh()->Play("StandIdle", 0.3f);
				// ���� state �������
				cStateAttack* pStateAttack = new cStateAttack;
				pStateAttack->SetTarget(m_pTarget);
				pStateAttack->SetPosition(m_pTarget->GetPosition());
				pStateAttack->SetDir(vDirection);
				pStateAttack->SetPlayerPosition(vPlayer);
				pStateAttack->SetDelegate(pStateAttack);
				pStateAttack->Start();

				m_pTarget->SetState(pStateAttack);

				SAFE_RELEASE(pStateAttack);
			}
			else
			{
				if (m_fDeltaTime < 2.f)
					return;

				m_fDeltaTime = 0.f;

				cStateMove* pStateMove = new cStateMove;
				pStateMove->SetFrom(m_pTarget->GetPosition());
				pStateMove->SetTarget(m_pTarget);
				pStateMove->SetBuildings(m_pTarget->GetBuildings());

				/*D3DXVECTOR3 vDir = m_pTarget->GetPosition() - vPlayer;
				D3DXVec3Normalize(&vDir, &vDir);
*/
				pStateMove->SetTo(/*vDir * 5 + */vPlayer);	// vplayer�� ��ġ���� 8��ŭ ������ ������ �����ؾ���.
				pStateMove->Start();
				pStateMove->SetDelegate(pStateMove);

				m_pTarget->SetState(pStateMove);
				m_pTarget->GetMesh()->Play("Walk", 0.3f);
				SAFE_RELEASE(pStateMove);
			}
		}
		else
		{
			
			cStateMove* pStateMove = new cStateMove;
			pStateMove->SetFrom(m_pTarget->GetPosition());
			pStateMove->SetTarget(m_pTarget);
			pStateMove->SetBuildings(m_pTarget->GetBuildings());
			pStateMove->Start();
			pStateMove->SetDelegate(pStateMove);

			m_pTarget->SetState(pStateMove);
			m_pTarget->GetMesh()->Play("Walk", 0.3f);
			SAFE_RELEASE(pStateMove);
			
		}
	}
	else
	{
		if (m_pTarget->GetState()->GetStateType() == STATE_ATTACK)
		{
			cStateMove* pStateMove = new cStateMove;
			pStateMove->SetFrom(m_pTarget->GetPosition());
			pStateMove->SetTarget(m_pTarget);
			pStateMove->SetBuildings(m_pTarget->GetBuildings());
			pStateMove->Start();
			pStateMove->SetDelegate(pStateMove);

			m_pTarget->SetState(pStateMove);
			m_pTarget->GetMesh()->Play("Walk");
			SAFE_RELEASE(pStateMove);
		}
	}
	//	/*if (m_pTarget->GetState()->GetStateType() == STATE_ATTACK)
	//	return;
	//	*/
	//	if (m_pTarget->GetMesh()->IsPlay("StandFire"))
	//		return;
	//	
	//	D3DXVECTOR3 vPlayerDir;
	//	vPlayerDir = vPlayer - vTargetPos;
	//	D3DXVec3Normalize(&vPlayerDir, &vPlayerDir);

	//	if (D3DXVec3Dot(&vDirection, &vPlayerDir) < 0.5f)	// player�� �þ߰� �ȿ� ����
	//	{
	//		cStateMove* pStateMove = new cStateMove;
	//		pStateMove->SetFrom(m_pTarget->GetPosition());
	//		pStateMove->SetTarget(m_pTarget);
	//		pStateMove->SetBuildings(m_pTarget->GetBuildings());
	//		pStateMove->Start();
	//		pStateMove->SetDelegate(pStateMove);

	//		m_pTarget->SetState(pStateMove);
	//		SAFE_RELEASE(pStateMove);

	//		return;
	//	}

	//	if (fPlayerDist < m_fAttackRange - 1.5f)	// ���� �����Ÿ� �ȿ� ����
	//	{
	//		cStateAttack* pStateAttack = new cStateAttack;
	//		pStateAttack->SetTarget(m_pTarget);
	//		pStateAttack->SetPosition(m_pTarget->GetPosition());
	//		pStateAttack->SetDir(vDirection);
	//		pStateAttack->SetPlayerPosition(vPlayer);
	//		pStateAttack->SetDelegate(pStateAttack);
	//		pStateAttack->Start();

	//		m_pTarget->SetState(pStateAttack);
	//	
	//		SAFE_RELEASE(pStateAttack);		

	//		return;
	//	}
	//	else
	//	{
	//		cStateMove* pStateMove = new cStateMove;
	//		pStateMove->SetFrom(m_pTarget->GetPosition());
	//		pStateMove->SetTarget(m_pTarget);
	//		pStateMove->SetBuildings(m_pTarget->GetBuildings());
	//		pStateMove->SetTo(vPlayer);
	//		pStateMove->Start();
	//		pStateMove->SetDelegate(pStateMove);

	//		m_pTarget->SetState(pStateMove);
	//		SAFE_RELEASE(pStateMove);
	//	}
	//}
	

	//if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	//{
	//	D3DXVECTOR3 vDir;
	//	vDir = vPlayer - m_pTarget->GetPosition();

	//	D3DXVec3Normalize(&vDir, &vDir);

	//	m_pTarget->BulletFire(vDir);
	//}

}
