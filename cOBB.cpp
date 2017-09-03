#include "StdAfx.h"
#include "cOBB.h"
#include "cSkinnedMesh.h"
#include "cStaticMesh.h"

cOBB::cOBB(void)
{
	D3DXMatrixIdentity(&m_matWorldTM);
}


cOBB::~cOBB(void)
{
}

/// : 함수 인자로 넘겨주는 경우와 인자가 없는 경우 둘다 고려해 볼것
void cOBB::Setup( cSkinnedMesh* pSkinnedMesh, float scale)
{
	D3DXVECTOR3 vMin = pSkinnedMesh->GetMin() * scale;
	D3DXVECTOR3 vMax = pSkinnedMesh->GetMax() * scale;
	m_vOrgCenterPos = (vMin + vMax) / 2.f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;
}

void cOBB::Setup(cStaticMesh * pStaticMesh, float scale)
{
	D3DXVECTOR3 vMin = pStaticMesh->GetMin() * scale;
	D3DXVECTOR3 vMax = pStaticMesh->GetMax() * scale;
	m_vOrgCenterPos = (vMin + vMax) / 2.f;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	SetVertex();
}

void cOBB::Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, float scale)
{
	m_vOrgCenterPos = (vMin + vMax) / 2.f * scale;

	m_vOrgAxisDir[0] = D3DXVECTOR3(1, 0, 0);
	m_vOrgAxisDir[1] = D3DXVECTOR3(0, 1, 0);
	m_vOrgAxisDir[2] = D3DXVECTOR3(0, 0, 1);

	m_fAxisLen[0] = fabs(vMax.x - vMin.x);
	m_fAxisLen[1] = fabs(vMax.y - vMin.y);
	m_fAxisLen[2] = fabs(vMax.z - vMin.z);

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;

	SetVertex();
}

void cOBB::Update( D3DXMATRIXA16* pmatWorld )
{
	if(pmatWorld)
		m_matWorldTM = *pmatWorld;

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(
			&m_vAxisDir[i],
			&m_vOrgAxisDir[i],
			&m_matWorldTM);
	}

	D3DXVec3TransformCoord(
		&m_vCenterPos,
		&m_vOrgCenterPos,
		&m_matWorldTM);

	m_fRadius = D3DXVec3Length(&(m_vCenterPos - m_vAxisDir[0])) / 2;

	SetVertex();
}

bool cOBB::IsCollision( cOBB* pOBB1, cOBB* pOBB2 )
{
	float cos[3][3];		
	float absCos[3][3];
	float dist[3];
	float r0, r1, r;

	const float cutOff = 0.999999f;
	bool existsParallelPair = false;

	D3DXVECTOR3 D = pOBB2->m_vCenterPos - pOBB1->m_vCenterPos;

	/// :  OBB1 의 3 축 (a) 을 기준으로 OBB2 의 3축(b) 을 검사
	for( int a = 0 ; a < 3 ; a++ )
	{
		for( int b = 0 ; b < 3 ; b++ )
		{
			// : OBB_A a 축과 OBB_B b 축의 cos 값
			cos[a][b] = D3DXVec3Dot( &pOBB1->m_vAxisDir[a], &pOBB2->m_vAxisDir[b] );

			/// : 위에서 구한 cos 값의 절대값
			absCos[a][b] = abs( cos[a][b] );


			/// : 한축이 직각이다 이러한경우 AABB 형태
			if( absCos[a][b] > cutOff )
				existsParallelPair = true;
		}


		/// : 사각형 끼리의 중심 거리 벡터를 OBB1 의 a 축으로 투영한 거리
		dist[a] = D3DXVec3Dot( &pOBB1->m_vAxisDir[a], &D );
		r = abs( dist[a] );

		/// : OBB1 의 a 축의 길이
		r0 = pOBB1->m_fAxisHalfLen[a];

		/// : OBB2 의 꼭지점이 OBB1 의a 축으로 투영된 길이
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[a][0] + 
			pOBB2->m_fAxisHalfLen[1] * absCos[a][1] +
			pOBB2->m_fAxisHalfLen[2] * absCos[a][2];

		if( r > r0 + r1 )
			return false;
	}

	/// OBB2 의 3축을 기준으로 OBB1 의 3축을 검사
	for( int b = 0 ; b < 3 ; b++ )
	{
		r = abs( D3DXVec3Dot( &pOBB2->m_vAxisDir[b], &D ) );
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[0][b] + 
			pOBB1->m_fAxisHalfLen[1] * absCos[1][b] +
			pOBB1->m_fAxisHalfLen[2] * absCos[2][b];

		r1 = pOBB2->m_fAxisHalfLen[b];

		if( r > r0 + r1 )
			return false;
	}


	if( existsParallelPair ) return true;

	/////////////////////////////////////////////////////////////////
	{	// : 3 x 3 개 
		r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][2] + pOBB2->m_fAxisHalfLen[2] * absCos[1][0];
		if (r > r0 + r1)
			return false;

		r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[1][1] + pOBB2->m_fAxisHalfLen[1] * absCos[1][0];
		if (r > r0 + r1)
			return false;

		/////////////////////////////////////////////////////////////////	 

		r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][0] + pOBB1->m_fAxisHalfLen[1] * absCos[0][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][1];
		if (r > r0 + r1)
			return false;

		r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][1] + pOBB1->m_fAxisHalfLen[1] * absCos[0][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][2] + pOBB2->m_fAxisHalfLen[2] * absCos[2][0];
		if (r > r0 + r1)
			return false;


		r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
		r0 = pOBB1->m_fAxisHalfLen[0] * absCos[1][2] + pOBB1->m_fAxisHalfLen[1] * absCos[0][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[2][1] + pOBB2->m_fAxisHalfLen[1] * absCos[2][0];
		if (r > r0 + r1)
			return false;
	
		/////////////////////////////////////////////////////////////////

		r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][0] + pOBB1->m_fAxisHalfLen[2] * absCos[1][0];
		r1 = pOBB2->m_fAxisHalfLen[1] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][1];
		if (r > r0 + r1)
			return false;



		r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][1] + pOBB1->m_fAxisHalfLen[2] * absCos[1][1];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][2] + pOBB2->m_fAxisHalfLen[2] * absCos[0][0];
		if (r > r0 + r1)
			return false;



		r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
		r0 = pOBB1->m_fAxisHalfLen[1] * absCos[2][2] + pOBB1->m_fAxisHalfLen[2] * absCos[1][2];
		r1 = pOBB2->m_fAxisHalfLen[0] * absCos[0][1] + pOBB2->m_fAxisHalfLen[1] * absCos[0][0];
		if (r > r0 + r1)
			return false;
	} // << : 

	return true;
}

/// >> : 박스 그리기.
void cOBB::OBBBox_Render(D3DCOLOR c)
{
	std::vector<ST_PC_VERTEX> vecVertex;
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[0], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[1], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[1], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[2], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[2], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[3], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[3], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[0], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[0], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[4], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[1], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[5], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[2], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[6], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[3], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[7], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[4], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[5], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[5], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[6], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[6], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[7], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[7], c));
	vecVertex.push_back(ST_PC_VERTEX(m_vVertex[4], c));

	g_pD3DDevice->SetTexture(NULL,NULL);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		vecVertex.size() / 2,
		&vecVertex[0],
		sizeof(ST_PC_VERTEX));
}

void cOBB::SetScale(float scale)
{
	m_vOrgCenterPos *= scale;

	m_fAxisLen[0] *= scale;
	m_fAxisLen[1] *= scale;
	m_fAxisLen[2] *= scale;

	m_fAxisHalfLen[0] = m_fAxisLen[0] / 2.0f;
	m_fAxisHalfLen[1] = m_fAxisLen[1] / 2.0f;
	m_fAxisHalfLen[2] = m_fAxisLen[2] / 2.0f;
}

void cOBB::SetVertex()
{
	m_vVertex[0] = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0],
		m_vOrgCenterPos.y - m_fAxisHalfLen[1],
		m_vOrgCenterPos.z - m_fAxisHalfLen[2]);
	m_vVertex[1] = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0],
		m_vOrgCenterPos.y + m_fAxisHalfLen[1],
		m_vOrgCenterPos.z - m_fAxisHalfLen[2]);
	m_vVertex[2] = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0],
		m_vOrgCenterPos.y + m_fAxisHalfLen[1],
		m_vOrgCenterPos.z - m_fAxisHalfLen[2]);
	m_vVertex[3] = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0],
		m_vOrgCenterPos.y - m_fAxisHalfLen[1],
		m_vOrgCenterPos.z - m_fAxisHalfLen[2]);

	m_vVertex[4] = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0],
		m_vOrgCenterPos.y - m_fAxisHalfLen[1],
		m_vOrgCenterPos.z + m_fAxisHalfLen[2]);
	m_vVertex[5] = D3DXVECTOR3(m_vOrgCenterPos.x - m_fAxisHalfLen[0],
		m_vOrgCenterPos.y + m_fAxisHalfLen[1],
		m_vOrgCenterPos.z + m_fAxisHalfLen[2]);
	m_vVertex[6] = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0],
		m_vOrgCenterPos.y + m_fAxisHalfLen[1],
		m_vOrgCenterPos.z + m_fAxisHalfLen[2]);
	m_vVertex[7] = D3DXVECTOR3(m_vOrgCenterPos.x + m_fAxisHalfLen[0],
		m_vOrgCenterPos.y - m_fAxisHalfLen[1],
		m_vOrgCenterPos.z + m_fAxisHalfLen[2]);

	D3DXVec3TransformCoord(&m_vWorldVertex[0], &m_vVertex[0], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[1], &m_vVertex[1], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[2], &m_vVertex[2], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[3], &m_vVertex[3], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[4], &m_vVertex[4], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[5], &m_vVertex[5], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[6], &m_vVertex[6], &m_matWorldTM);
	D3DXVec3TransformCoord(&m_vWorldVertex[7], &m_vVertex[7], &m_matWorldTM);
}
