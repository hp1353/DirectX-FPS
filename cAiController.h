#pragma once
#include "cController.h"

class cAI;
class cAIController
	: public cController
{
	cAI*			m_pTarget;

	float			m_fAttackRange;
	float			m_fDeltaTime;
	
public:
	cAIController();
	~cAIController();

	void Setup(float moveSpeed, cCharacter* pCharacter = NULL) override;
	void Update(OUT D3DXVECTOR3& vPlayer, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) override;

};

