#include "StdAfx.h"
#include "cRay.h"


cRay::cRay(void)
	: m_vOrg(0, 0, 0)
	, m_vDir(0, 0, 0)
	, m_eRaySpace(E_NONE)
{
}


cRay::~cRay(void)
{
}

cRay cRay::RayAtViewSpace( int nScreenX, int nScreenY )
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIXA16 matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	cRay r;
	r.m_vDir.x = (( 2.0 * nScreenX) / vp.Width  - 1.0f) / matProj._11;
	r.m_vDir.y = ((-2.0 * nScreenY) / vp.Height + 1.0f) / matProj._22;
	r.m_vDir.z = 1.0f;

	r.m_eRaySpace = E_VIEW;

	return r;
}

cRay cRay::RayAtWorldSpace( int nScreenX, int nScreenY )
{
	cRay r = cRay::RayAtViewSpace(nScreenX, nScreenY);

	assert(r.m_eRaySpace == E_VIEW);

	D3DXMATRIXA16 matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matInvView, 0, &matView);

	D3DXVec3TransformCoord(&r.m_vOrg, &r.m_vOrg, &matInvView);
	D3DXVec3TransformNormal(&r.m_vDir, &r.m_vDir, &matInvView);

	D3DXVec3Normalize(&r.m_vDir, &r.m_vDir);

	r.m_eRaySpace = E_WORLD;

	return r;
}

bool cRay::IntersectTri( IN D3DXVECTOR3& v0, 
	IN D3DXVECTOR3& v1,
	IN D3DXVECTOR3& v2,
	OUT D3DXVECTOR3& vPickedPosition )
{
	float u, v, t;
	bool b = D3DXIntersectTri(&v0, &v1, &v2, &m_vOrg, &m_vDir, &u, &v, &t);
	vPickedPosition = m_vOrg + t * m_vDir;
	return b;
}

bool cRay::IsPicked( ST_SPHERE* pSphere )
{
	cRay r = (*this);

	assert(r.m_eRaySpace == E_WORLD);

	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixIdentity(&matInvWorld);
	matInvWorld._41 = -pSphere->vCenter.x;
	matInvWorld._42 = -pSphere->vCenter.y;
	matInvWorld._43 = -pSphere->vCenter.z;

	D3DXVec3TransformCoord(&r.m_vOrg, &r.m_vOrg, &matInvWorld);
	D3DXVec3TransformNormal(&r.m_vDir, &r.m_vDir, &matInvWorld);

	//r.m_eRaySpace = E_LOCAL;

	// (Q.V)^2 - V.V*(Q.Q - r*r) > 0
	float vv = D3DXVec3Dot(&r.m_vDir, &r.m_vDir);
	float qv = D3DXVec3Dot(&r.m_vOrg, &r.m_vDir);
	float qq = D3DXVec3Dot(&r.m_vOrg, &r.m_vOrg);
	float rr = pSphere->fRadius * pSphere->fRadius;

	return qv * qv - vv * (qq - rr) >= 0;
}
