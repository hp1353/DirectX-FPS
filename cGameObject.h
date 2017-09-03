#pragma once

#include "cState.h"

class cGameObject
	: public cObject
	, public iStateDelegate
{
protected:
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDirection, Direction);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE_ADD_REF(cState*, m_pState, State);

public:
	cGameObject(void);
	virtual ~cGameObject(void);

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void OnStateFinish(cState* pSender) override;
};

