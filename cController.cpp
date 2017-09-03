#include "stdafx.h"
#include "cController.h"


cController::cController()
	: m_fMoveSpeed(0.1f)
	, m_pOBB(NULL)
	, m_pHeightMap(NULL)
	, m_pTextMap(NULL)
{
}


cController::~cController()
{
}

void cController::SetOBB(cOBB * pObb)
{
	m_pOBB = pObb;
}

void cController::SetHeightMap(cHeightMap * pHeightMap)
{
	m_pHeightMap = pHeightMap;
}

void cController::SetTextMap(cTextMap * pTextMap)
{
	m_pTextMap = pTextMap;
}
