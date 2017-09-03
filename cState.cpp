#include "stdafx.h"
#include "cState.h"


cState::cState()
	: m_pTarget(NULL)
	, m_pDelegate(NULL)
	, m_eStateType(STATE_NULL)
{
}


cState::~cState()
{
}
