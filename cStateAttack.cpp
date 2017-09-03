#include "stdafx.h"
#include "cStateAttack.h"

#include "cAI.h"
#include "cStateMove.h"

cStateAttack::cStateAttack()
	: m_fPassedTime(0.f)
{
}


cStateAttack::~cStateAttack()
{
}

void cStateAttack::Start()
{
	if (!m_pTarget) return;

	m_fPassedTime = 0.f;

	m_eStateType = STATE_ATTACK;

	
}

void cStateAttack::Update()
{
	m_fPassedTime += g_pTimeManager->GetElapsedTime();

	if (m_fPassedTime > 1.5f)
	{
		m_vDir = m_vPlayerPosition - m_vPosition;

		D3DXVec3Normalize(&m_vDir, &m_vDir);
		((cAI*)m_pTarget)->BulletFire(m_vDir);
		m_pTarget->SetPosition(m_vPosition);
		m_pTarget->SetDirection(m_vDir);

		if (m_pDelegate)
			m_pDelegate->OnStateFinish(this);
	}
}

void cStateAttack::OnStateFinish(cState * pSender)
{
	Start();
}
