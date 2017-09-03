#include "stdafx.h"
#include "cAirDrop.h"
#include "cStaticMesh.h"
#include "cHeightMap.h"


cAirDrop::cAirDrop()
	: m_pAirdrop(NULL)
	, m_pParachute(NULL)
	, m_pAirMan(NULL)
	, m_pHeightMap(NULL)
	, m_fGravity(0.001f)
	, m_isPlay(true)
{
}


cAirDrop::~cAirDrop()
{
	SAFE_DELETE(m_pAirdrop);
	SAFE_DELETE(m_pParachute);
	SAFE_DELETE(m_pAirMan);
}

void cAirDrop::Setup(cHeightMap* heightMap)
{
	m_pHeightMap = heightMap;

	m_vPos = D3DXVECTOR3(0, 30, 0);
	m_vCamPos = D3DXVECTOR3(0, 32, 0);

	m_pAirdrop = new cStaticMesh("Airdrop/", "Airdrop.X");
	m_pParachute = new cStaticMesh("Airdrop/", "Parachute.X");
	m_pAirMan = new cStaticMesh("Airdrop/", "AirMan.X");
	
	D3DXMATRIXA16 matS, matR, matT, matSRT;

	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	matSRT = matS * matR * matT;

	m_pAirdrop->SetWorld(matSRT);
	m_pParachute->SetWorld(matSRT);
	m_pAirMan->SetWorld(matSRT);

}

void cAirDrop::Update()
{
	m_vPos.y -= (0.03f + m_fGravity);
	m_vCamPos.y -= (0.03f + m_fGravity);

	m_fGravity += 0.001f;

	if (m_pHeightMap)
	{
		float height = 0.f;
		m_pHeightMap->GetHeight(m_vPos.x, height, m_vPos.z);
		
		if (m_vPos.y < height)
		{
			m_vPos.y = height;
			m_vCamPos.y = height + 2;
			m_isPlay = false;
		}
	}

	D3DXMATRIXA16 matS, matR, matT, matSRT;

	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	D3DXMatrixTranslation(&matT, m_vPos.x, m_vPos.y, m_vPos.z);

	matSRT = matS * matR * matT;

	m_pAirdrop->SetWorld(matSRT);
	m_pParachute->SetWorld(matSRT);
	m_pAirMan->SetWorld(matSRT);

}

void cAirDrop::Render()
{
	if (m_pAirdrop)
		m_pAirdrop->Render();

	if (m_pParachute)
		m_pParachute->Render();

	if (m_pAirMan)
		m_pAirMan->Render();
}
