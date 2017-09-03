#pragma once

class cSkinnedMesh;
class cBullet;
class cEffect;

class cGun : public cObject
{
	cSkinnedMesh*				m_pGun;				// Gun�� �޽�
	D3DXVECTOR3*				m_pvTarget;			// Gun�� ������ object�� ��ġ

	std::vector<cBullet*>		m_pvBullet;			// �Ѿ� ������

	D3DXMATRIXA16				m_pWorldTM;
	D3DXMATRIXA16				m_pTrans;
	D3DXMATRIXA16				m_pTransInv;

	std::vector<cEffect*>		m_pvEffect;

	SYNTHESIZE(float, m_fAttackPower, AttackPower);		// ���� ����
	SYNTHESIZE(float, m_fAttackRange, AttackRange);		// ��Ÿ�
	SYNTHESIZE(float, m_fAttackSpeed, AttackSpeed);		// ����(����) �ӵ�

	SYNTHESIZE(float, m_fCurrentExp, CurrentExp);		// �ѱ� �ִ� ����ġ
	SYNTHESIZE(float, m_fMaxExp, MaxExp);		        // �ѱ� ���� ����ġ
	SYNTHESIZE(int, m_CurrentLv, CurrentLv);		    // �ѱ� ���� ����
	SYNTHESIZE(int, m_MaxLv, MaxLv);		            // �ѱ� �ִ� ����

	SYNTHESIZE(int, m_nMaxBullet, MaxBullet);			// �Ѿ� �ִ�
	SYNTHESIZE(int, m_nCurrentBullet, CurrentBullet);	// ���� ���� �Ѿ� ��
	SYNTHESIZE(int, m_nMagazine, Magazine);	            // �ѹ��� ���� �� ���ִ� źâ ��



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

