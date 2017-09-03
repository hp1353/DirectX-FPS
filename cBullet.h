#pragma once

class cBullet : public cGameObject
{
	ST_SPHERE*					m_pSphere;
	std::vector<ST_PC_VERTEX>	m_vecVertex;				// 파티클 Vertex

	float						m_fBulletSpeed;				// 총알의 이동 속도

	LPD3DXMESH					m_pBoundingSphereMesh;
	ST_SPHERE					m_stBoundingSphere;

	SYNTHESIZE(float, m_fMoveDistance, MoveDistance);		// 총알이 이동한 거리

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

