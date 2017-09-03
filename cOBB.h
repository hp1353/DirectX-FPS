#pragma once
class cStaticMesh;
class cOBB
{
private:
	D3DXVECTOR3 m_vOrgCenterPos;		
	D3DXVECTOR3 m_vOrgAxisDir[3];		
	
	D3DXVECTOR3 m_vCenterPos;		/// :  �ڽ� �߽�r ��ǥ
	D3DXVECTOR3 m_vAxisDir[3];		/// :  �ڽ��� ������ �� ���� ��������
	float		m_fAxisLen[3];		/// :  �ڽ��� ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	float		m_fAxisHalfLen[3];

	D3DXVECTOR3 m_vVertex[8];
	D3DXVECTOR3 m_vWorldVertex[8];

	D3DXMATRIXA16 m_matWorldTM;

	float		m_fRadius;
public:
	cOBB(void);
	~cOBB(void);

	void Setup(cSkinnedMesh* pSkinnedMesh, float scale = 1.0f);
	void Setup(cStaticMesh* pStaticMesh, float scale = 1.0f);
	void Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax, float scale = 1.0f);
	void Update(D3DXMATRIXA16* pmatWorld);
	static bool IsCollision(cOBB* pOBB1, cOBB* pOBB2);

	void OBBBox_Render(D3DCOLOR c); 

	D3DXMATRIXA16* GetWorldTM() { return &m_matWorldTM; }
	void SetScale(float scale);
	void SetVertex();

	D3DXVECTOR3* GetVertes() { return m_vWorldVertex; }
	float GetRadius() { return m_fRadius; }
};

