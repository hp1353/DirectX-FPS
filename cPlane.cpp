#include "stdafx.h"
#include "cPlane.h"
#include "cStaticMesh.h"


cPlane::cPlane()
	: m_pPlane(NULL)
	, planeZ(rand() % 50)
{
}


cPlane::~cPlane()
{
	SAFE_DELETE(m_pPlane);
//	m_pPlane->Destroy();
}
void cPlane::Setup(char* szFolder, char* szFilename)
{
	m_pPlane = new cStaticMesh(szFolder, szFilename);
}

void cPlane::Update()
{
	planeZ += 0.01f;

	D3DXMATRIXA16 matS, matT, matST;
	D3DXMatrixScaling(&matS, 0.001f, 0.001f, 0.001f);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, planeZ);
	matST = matS * matT;
	m_matWorld = matS * matT;

	m_pPlane->SetWorld(m_matWorld);

	if (planeZ > 40.f)
		planeZ = -40.f;

}

void cPlane::Render()
{
	if(m_pPlane)
		m_pPlane->Render();
}

void cPlane::SetPosition(D3DXVECTOR3 pos)
{
	m_vPosition = pos;
}
