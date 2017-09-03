#pragma once
class cRay
{
public:
	enum eRaySpace
	{
		E_NONE,
		E_VIEW,
		E_WORLD,
		E_LOCAL
	};

protected:
	//D3DXVECTOR3 m_vOrg;
	//D3DXVECTOR3 m_vDir;
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vOrg, RayOrg);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vDir, RayDir);
	eRaySpace	m_eRaySpace;

public:
	cRay(void);
	~cRay(void);

	static cRay RayAtViewSpace(int nScreenX, int nScreenY);
	static cRay RayAtWorldSpace(int nScreenX, int nScreenY);

	bool IntersectTri(IN D3DXVECTOR3& v0, 
		IN D3DXVECTOR3& v1,
		IN D3DXVECTOR3& v2,
		OUT D3DXVECTOR3& vPickedPosition);

	bool IsPicked(ST_SPHERE* pSphere);
};

