#pragma once

class cBullet : public cGameObject
{
	ST_SPHERE*					m_pSphere;
	std::vector<ST_PC_VERTEX>	m_vecVertex;				// ��ƼŬ Vertex

	float						m_fBulletSpeed;				// �Ѿ��� �̵� �ӵ�

	LPD3DXMESH					m_pBoundingSphereMesh;
	ST_SPHERE					m_stBoundingSphere;

	SYNTHESIZE(float, m_fMoveDistance, MoveDistance);		// �Ѿ��� �̵��� �Ÿ�

public:
	cBullet();
	~cBullet();

	void Setup(D3DXVECTOR3 vDirection, D3DXVECTOR3 vPosition);
	void Update();
	void Render();

	ST_SPHERE GetBoundingSphere() { return m_stBoundingSphere; }

	/// >> : particle
	void Setup_Particle();
	void Update_Particle();
	void Render_Particle();
	/// << : 
};

