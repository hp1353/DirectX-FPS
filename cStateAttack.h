#pragma once

#include "cState.h"
class cAI;

class cStateAttack
	: public cState
	, public iStateDelegate
{
	float		m_fPassedTime;

	SYNTHESIZE(D3DXVECTOR3, m_vDir, Dir);
	SYNTHESIZE(D3DXVECTOR3, m_vPlayerPosition, PlayerPosition);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cStateAttack();
	virtual ~cStateAttack();

	virtual void Start() override;
	virtual void Update() override;

	// iStateDelegate overrride
	virtual void OnStateFinish(cState* pSender) override;
};

