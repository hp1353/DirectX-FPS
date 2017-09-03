#pragma once

struct ST_BONE;

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	ST_BONE*					m_pRootFrame;
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	
	D3DXMATRIXA16				m_matWorldTM;
	D3DXMATRIXA16				m_matBoneWorldTM;
	D3DXMATRIXA16				m_matLocalTM;

	/// >> : OBB -
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);

	SYNTHESIZE(D3DXMATRIXA16*, m_pmatParent, Parent);
	ST_SPHERE					m_stBoundingSphere;

	

	UINT										m_AnimNum;
	std::map< std::string, LPD3DXANIMATIONSET > m_mapAnimSet;
	std::vector< LPD3DXANIMATIONSET >			m_vecAnimSet;

	LPD3DXANIMATIONSET				m_pNowPlayAnimationSet;		//���� �÷��̵ǰ��ִ� AnimationSet

	D3DXTRACK_DESC					m_Track_Desc_0;				//0�� Track_Desc ( 0 �� Track �� ���� )

	bool							m_bPlay;					//���� �÷��� ���̴�?
	bool							m_bLoop;					//���� �÷��̴�?

	float							m_fCrossFadeTime;			//ũ�ν����̵� Ÿ��
	float							m_fLeftCrossFadeTime;		//����ũ�ν����̵� Ÿ��

	LPD3DXANIMATIONSET				m_pPrevPlayAnimationSet;	//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton
	float							m_fOutCrossFadeTime;		//OneShot Animation ������ �ǵ��ư��� CrossFade Ÿ��
	double							m_AnimationPlayFactor;		//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )

	float							m_fAnimDelta;				//Animation �����ų �������Ӵ� ��
	float							m_fPassedTime;

public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	~cSkinnedMesh(void);
	
	void Update();
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);

	void SetRandomTrackPosition();
	void SetTransform(D3DXMATRIXA16* pmat);

	D3DXMATRIXA16* GetTransForm();

	void ResetAndSetAnimationIndex(int nIndex);
	
	ST_SPHERE* GetBoundingSphere();
	
	D3DXMATRIXA16* getMatrix(char * name);
	D3DXMATRIXA16* getTransformationMatrix(char * name);
	D3DXMATRIXA16* getWorldMatrix(char * name);
	D3DXMATRIXA16* getLocalMatrix(char * name);

	ST_BONE* getBone(char * name, ST_BONE * pBone = NULL);

public:	// animation function

	void Play(std::string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);

	void PlayOneShot(std::string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShot(int animIndex, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(std::string animName, float crossFadeTime = 0.0);
	void PlayOneShotAfterHold(int animIndex, float crossFadeTime = 0.0);

	void Pause() { m_bPlay = false;	}
	void Resume() { m_bPlay = true; }
	bool IsPlay() {	return m_bPlay; }
	bool IsPlay(std::string animName);

	//�ִϸ��̼� �ӵ�����
	void SetPlaySpeed(float speed);

	//�ش� �ε����� Animation Set �� ��´�.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return m_vecAnimSet[index];
	}

private:
	cSkinnedMesh();

	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void UpdateLocal(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();

	void SetAnimation(LPD3DXANIMATIONSET animSet);
};

