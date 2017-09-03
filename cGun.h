#pragma once

class cSkinnedMesh;
class cBullet;
class cEffect;

class cGun : public cObject
{
	cSkinnedMesh*				m_pGun;				// Gun의 메쉬
	D3DXVECTOR3*				m_pvTarget;			// Gun을 가지는 object의 위치

	std::vector<cBullet*>		m_pvBullet;			// 총알 여러개

	D3DXMATRIXA16				m_pWorldTM;
	D3DXMATRIXA16				m_pTrans;
	D3DXMATRIXA16				m_pTransInv;

	std::vector<cEffect*>		m_pvEffect;

	SYNTHESIZE(float, m_fAttackPower, AttackPower);		// 공격 세기
	SYNTHESIZE(float, m_fAttackRange, AttackRange);		// 사거리
	SYNTHESIZE(float, m_fAttackSpeed, AttackSpeed);		// 공격(연사) 속도

	SYNTHESIZE(float, m_fCurrentExp, CurrentExp);		// 총기 최대 경험치
	SYNTHESIZE(float, m_fMaxExp, MaxExp);		        // 총기 현재 경험치
	SYNTHESIZE(int, m_CurrentLv, CurrentLv);		    // 총기 현재 레벨
	SYNTHESIZE(int, m_MaxLv, MaxLv);		            // 총기 최대 레벨

	SYNTHESIZE(int, m_nMaxBullet, MaxBullet);			// 총알 최대
	SYNTHESIZE(int, m_nCurrentBullet, CurrentBullet);	// 현재 남은 총알 수
	SYNTHESIZE(int, m_nMagazine, Magazine);	            // 한번에 장착 할 수있는 탄창 수



	LPD3DXMESH				m_pMesh;
	D3DXMATRIXA16			m_matTagBarrel;


public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update();
	void Render();

	std::vector<cBullet*> GetBullets() { return m_pvBullet; }
	void SetParentWorldMatrix(D3DXMATRIXA16* matWorld);
	void SetWorldMatrix(D3DXMATRIXA16* matWorld);
	void SetWorldMatrixByBoneName(D3DXMATRIXA16* matRot, char* name);
	
	bool Fire(D3DXVECTOR3 vDirection, D3DXMATRIXA16& vPosition);
	void Reload();
	void RemoveBullet(int bulletIndex);

	bool IsShoot();
	bool IsReload();

	cSkinnedMesh* GetGunMesh();
};

