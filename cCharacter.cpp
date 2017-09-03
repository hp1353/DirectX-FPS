#include "StdAfx.h"
#include "cCharacter.h"
#include "cController.h"
#include "cGun.h"
#include "cOBB.h"

cCharacter::cCharacter(void)
	: m_pGun(NULL)
	, m_pController(NULL)
	, m_fMaxHp(8)
	, m_fCurrentHp(8)
{
	D3DXMatrixIdentity(&m_matWorldTM);
}


cCharacter::~cCharacter(void)
{
}

void cCharacter::Setup(char* szFolder, char* szFilename)
{
}

void cCharacter::Update()
{
	if (m_pGun)
		m_pGun->Update();
}

void cCharacter::Render()
{
	if (m_pGun)
		m_pGun->Render();
}

cGun * cCharacter::GetGun()
{
	if (m_pGun)
		return m_pGun;

	return NULL;
}

bool cCharacter::IsAttacked(float power)
{
	m_fCurrentHp -= power;

	return m_fCurrentHp < 0 ? true : false;
}
