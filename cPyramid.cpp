#include "StdAfx.h"
#include "cPyramid.h"


cPyramid::cPyramid(void)
	: m_pVB(NULL)
	, m_nNumTri(0)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cPyramid::~cPyramid(void)
{
	SAFE_RELEASE(m_pVB);
}

void cPyramid::Setup(D3DXCOLOR c, D3DXMATRIXA16& mat)
{
	std::vector<ST_PN_VERTEX>	vecVertex;

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = c;
	m_stMtl.Diffuse = c;
	m_stMtl.Specular = c;

	m_matWorld = mat;

	ST_PN_VERTEX v;

	v.p = D3DXVECTOR3( 0,  0,  0); vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 0,  0,  0); vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1, -1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); vecVertex.push_back(v);

	v.p = D3DXVECTOR3(-1, -1, -1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1, -1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); vecVertex.push_back(v);

	v.p = D3DXVECTOR3( 1, -1, -1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 1, -1,  1); vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-1, -1,  1); vecVertex.push_back(v);


	for (int i = 0; i < vecVertex.size(); i += 3)
	{
		D3DXVECTOR3 v01 = vecVertex[i + 1].p - vecVertex[i].p;
		D3DXVECTOR3 v02 = vecVertex[i + 2].p - vecVertex[i].p;
		D3DXVECTOR3 n;

		D3DXVec3Cross(&n, &v01, &v02);
		D3DXVec3Normalize(&n, &n);

		vecVertex[i].n = n;
		vecVertex[i + 1].n = n;
		vecVertex[i + 2].n = n;
	}

	m_nNumTri = vecVertex.size() / 3;
	g_pD3DDevice->CreateVertexBuffer(
		vecVertex.size() * sizeof(ST_PN_VERTEX),
		0,
		ST_PN_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	ST_PN_VERTEX* pV = NULL;
	m_pVB->Lock(0, 0, (LPVOID*)&pV, 0);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PN_VERTEX));
	m_pVB->Unlock();
}

void cPyramid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PN_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nNumTri);
}
