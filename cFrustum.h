#pragma once
class cFrustum
{
private:
	std::vector<D3DXVECTOR3>	m_vecProjVertex;
	std::vector<D3DXVECTOR3>	m_vecWorldVertex;
	std::vector<D3DXPLANE>		m_vecPlane;

public:
	cFrustum(void);
	~cFrustum(void);

	void Setup();
	void Update();
	bool IsIn(ST_SPHERE* pSphere);
	bool IsIn(D3DXVECTOR3 vPos, float radius = 1);
};

