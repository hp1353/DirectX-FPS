#pragma once
class cStaticMesh;
class cHeightMap;

class cAirDrop : public cObject
{
	cStaticMesh*				m_pAirdrop;
	cStaticMesh*				m_pParachute;
	cStaticMesh*				m_pAirMan;

	cHeightMap*					m_pHeightMap;

	float						m_fGravity;

	D3DXVECTOR3					m_vPos;
	bool						m_isPlay;

	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vCamPos, Position);

public:
	cAirDrop();
	~cAirDrop();

	void Setup(cHeightMap* heightMap);
	void Update();
	void Render();

	bool isPlay() { return m_isPlay; }
};

