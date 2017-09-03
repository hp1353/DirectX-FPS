#include "StdAfx.h"
#include "cGameObject.h"

cGameObject::cGameObject(void)
	: m_vDirection(0, 0, 1)
	, m_vPosition(0, 0, 0)
	, m_pState(NULL)
{
}

cGameObject::~cGameObject(void)
{
	SAFE_RELEASE(m_pState);
}

void cGameObject::Setup()
{
}

void cGameObject::Update()
{
	if (m_pState)
	{
		m_pState->Update();
	}
}

void cGameObject::Render()
{
}

void cGameObject::OnStateFinish(cState * pSender)
{

}
