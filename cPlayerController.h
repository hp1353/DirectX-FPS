#pragma once
#include "cController.h"

class cPlayer;

class cPlayerController 
	: public cController
{
	cPlayer*			m_pTarget;

public:
	cPlayerController();
	~cPlayerController();

	void Setup(float moveSpeed, cCharacter* pCharacter = NULL) override;
	void Update(D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) override;
};

