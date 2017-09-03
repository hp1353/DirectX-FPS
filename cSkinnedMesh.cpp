#include "StdAfx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"


cSkinnedMesh::cSkinnedMesh(char* szFolder, char* szFilename)
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_pmatParent(NULL)
	, m_fPassedTime(0.0f)
{
	cSkinnedMesh* pSkinnedMesh = g_pSkinnedMeshManager->GetSkinnedMesh(szFolder, szFilename);

	D3DXMatrixIdentity(&m_matWorldTM);
	D3DXMatrixIdentity(&m_matLocalTM);
	
	//m_pRootFrame = new ST_BONE;
	//memcpy(m_pRootFrame, pSkinnedMesh->m_pRootFrame, sizeof(ST_BONE));
	m_pRootFrame = pSkinnedMesh->m_pRootFrame;
	m_dwWorkingPaletteSize = pSkinnedMesh->m_dwWorkingPaletteSize;
	m_pmWorkingPalette = pSkinnedMesh->m_pmWorkingPalette;
	m_pEffect = pSkinnedMesh->m_pEffect;

	/// >> : OBB
	{
		m_vMin = pSkinnedMesh->m_vMin;
		m_vMax = pSkinnedMesh->m_vMax;
	}

	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);

	m_AnimNum = m_pAnimController->GetNumAnimationSets();

	for (UINT i = 0; i < m_AnimNum; i++)
	{
		LPD3DXANIMATIONSET animSet;
		m_pAnimController->GetAnimationSet(i, &animSet);

		m_vecAnimSet.push_back(animSet);
		m_mapAnimSet.insert(std::make_pair(animSet->GetName(), animSet));
	}

	Play(/*rand() % m_AnimNum*/0);
}


void cSkinnedMesh::ResetAndSetAnimationIndex(int nIndex)
{
	if (!m_pAnimController)
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	m_pAnimController->SetTrackPosition(0, 0.0f);

	SAFE_RELEASE(pAnimSet);
}

ST_SPHERE * cSkinnedMesh::GetBoundingSphere()
{
	return &m_stBoundingSphere;
}

D3DXMATRIXA16 * cSkinnedMesh::getMatrix(char * name)
{
	ST_BONE * pBone = getBone(name, m_pRootFrame);

	if (pBone != NULL)
		return &pBone->CombinedTransformationMatrix;

	return NULL;
}

D3DXMATRIXA16 * cSkinnedMesh::getTransformationMatrix(char * name)
{
	ST_BONE * pBone = getBone(name, m_pRootFrame);

	if (pBone != NULL)
		return &(D3DXMATRIXA16)pBone->TransformationMatrix;

	return NULL;
}

D3DXMATRIXA16 * cSkinnedMesh::getWorldMatrix(char * name)
{
	ST_BONE * pBone = getBone(name, m_pRootFrame);

	if (pBone != NULL)
	{
		D3DXMATRIXA16 TransformationInv;
		D3DXMatrixInverse(&TransformationInv, 0, &(D3DXMATRIXA16)pBone->TransformationMatrix);
		m_matBoneWorldTM = *cSkinnedMesh::getMatrix(name) * TransformationInv;

		return &m_matBoneWorldTM;
	}

	return NULL;
}

D3DXMATRIXA16 * cSkinnedMesh::getLocalMatrix(char * name)
{
	ST_BONE * pBone = getBone(name, m_pRootFrame);

	if (pBone != NULL)
		return &pBone->LocalTransformationMatrix;

	return NULL;
}

ST_BONE * cSkinnedMesh::getBone(char * name, ST_BONE * pBone)
{
	LPD3DXFRAME pFrame = D3DXFrameFind(
		m_pRootFrame,			//��Ʈ ������
		name		//ã�� ���̸� ����
	);

	if (pFrame != NULL)
		return (ST_BONE*)pFrame;

	return NULL;
}

void cSkinnedMesh::Update()
{
	m_fPassedTime += g_pTimeManager->GetElapsedTime();
	//0 �� Track ������ ��´�( �������Ӹ��� �����ؾ� �Ѵ� )
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	//m_pNowPlayAnimationSet->GetPeriod();		//���� �ִϸ��̼��� ��ü ����
	//m_Track_Desc_0.Position;					//Track 0 ���� �÷��̵ǰ��ִ� Animation �÷��� Position

	//���� Animation Play �Ⱓ�� 0 ~ 1 �� ������ ���൵....
	//���� ���� ������ ( 1 �� �Ѿ )
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();


	//Loop �� �ȵǴ� Animation �� �ѹ� �÷��̵ǰ� ������...
	if (m_AnimationPlayFactor >= 0.99 && this->m_bLoop == false)
	{
		m_fPassedTime = 0.f;
		//Ʈ���� ���������� ����
		m_pAnimController->SetTrackPosition(0, m_Track_Desc_0.Priority);

		//���ư� Animation �� �ִٸ�..
		if (this->m_pPrevPlayAnimationSet != NULL)
		{
			m_fCrossFadeTime = m_fOutCrossFadeTime;
			m_fLeftCrossFadeTime = m_fOutCrossFadeTime;
			m_bLoop = true;
			this->SetAnimation(m_pPrevPlayAnimationSet);
			this->m_pPrevPlayAnimationSet = NULL;
		}

		//���ư� Animation �� ���ٸ�.
		else
		{
			this->m_bPlay = false;
		}

	}




	if (m_bPlay) {

		//�ν��Ͻ̵� SkinnedAnimation Ŭ�������� AdvanceTime ������ѹ�����
		//�ϳ��ۿ����� ���� �ش� SkinnedAnimation �ν��Ͻ��� Animation ������ 
		//������Ʈ �ȴ�
		//���� �ν��Ͻ̵� SkinnedAnimation �� ������Ʈ ���� ����س��Ҵٰ�
		//Rendering �ɶ� AdvanceTime ���� �����Ų��.
		m_fAnimDelta = m_fPassedTime;
	}
	else {
		m_fAnimDelta = 0.0f;
		m_fPassedTime = 0.f;
	}



	//ũ�ν� ���̵尡 �������̶��..
	if (m_fLeftCrossFadeTime > 0.0f)	//���� ũ�ν����̵� Ÿ���� �����ϴ°� Play �Ҷ� �����̵ȴ�.
	{
		//���� ũ�ν����̵� �ð� ����
		this->m_fLeftCrossFadeTime -= m_fPassedTime;

		//ũ�ν����̵� ��������.
		if (m_fLeftCrossFadeTime <= 0.0f)
		{
			//���� ��
			m_fLeftCrossFadeTime = 0.0f;
			m_pAnimController->SetTrackWeight(0, 1);		//0 �� Track ����ġ 1
			m_pAnimController->SetTrackEnable(1, false);	//1 �� Track ��Ȱ��ȭ
			
			m_fPassedTime = 0.f;
		}

		else
		{
			float w1 = (m_fLeftCrossFadeTime / m_fCrossFadeTime);		//1�� Track ����ġ
			float w0 = 1.0f - w1;										//0�� Track ����ġ

																		//����ġ ����
			m_pAnimController->SetTrackWeight(0, w0);
			m_pAnimController->SetTrackWeight(1, w1);
		}
	}
}

void cSkinnedMesh::Play(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	//�ʿ��� ������ ó��
	std::map< std::string, LPD3DXANIMATIONSET>::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end()) {

		this->m_pPrevPlayAnimationSet = NULL;

		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		m_fPassedTime = 0.f;

		this->SetAnimation(find->second);
	}
}

void cSkinnedMesh::Play(int animIndex, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	//���Ϳ��� ������ ó��
	if (animIndex < this->m_AnimNum) {

		this->m_pPrevPlayAnimationSet = NULL;

		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;


		this->SetAnimation(m_vecAnimSet[animIndex]);
	}
}

void cSkinnedMesh::PlayOneShot(std::string animName, float inCrossFadeTime, float outCrossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	std::map< std::string, LPD3DXANIMATIONSET>::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end()) {

		if (this->m_pPrevPlayAnimationSet == NULL)
		{
			//���� Animaiton �� ����Ѵ�.
			this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;
		}


		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = inCrossFadeTime;
		m_fLeftCrossFadeTime = inCrossFadeTime;

		//������ ũ�ν����̵� Ÿ�� ���
		m_fOutCrossFadeTime = outCrossFadeTime;

		this->SetAnimation(find->second);
	}
}

void cSkinnedMesh::PlayOneShot(int animIndex, float inCrossFadeTime, float outCrossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	if (this->m_pPrevPlayAnimationSet == NULL)
	{
		//���� Animaiton �� ����Ѵ�.
		this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;
	}


	//ũ�ν� ���̵� Ÿ�� ���
	m_fCrossFadeTime = inCrossFadeTime;
	m_fLeftCrossFadeTime = inCrossFadeTime;

	//������ ũ�ν����̵� Ÿ�� ���
	m_fOutCrossFadeTime = outCrossFadeTime;

	this->SetAnimation(m_vecAnimSet[animIndex]);

}

void cSkinnedMesh::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	std::map< std::string, LPD3DXANIMATIONSET>::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end()) {

		//���ư� Animation �� �������� �ʴ´�.
		this->m_pPrevPlayAnimationSet = NULL;

		//ũ�ν� ���̵� Ÿ�� ���
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void cSkinnedMesh::PlayOneShotAfterHold(int animIndex, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	//���ư� Animation �� �������� �ʴ´�.
	this->m_pPrevPlayAnimationSet = NULL;

	//ũ�ν� ���̵� Ÿ�� ���
	m_fCrossFadeTime = crossFadeTime;
	m_fLeftCrossFadeTime = crossFadeTime;

	this->SetAnimation(m_vecAnimSet[animIndex]);
}

bool cSkinnedMesh::IsPlay(std::string animName)
{
	return (animName == m_pNowPlayAnimationSet->GetName());
}

void cSkinnedMesh::SetPlaySpeed(float speed)
{
	m_pAnimController->SetTrackSpeed(0, speed);
}

cSkinnedMesh::cSkinnedMesh()
	: m_pRootFrame(NULL)
	, m_pAnimController(NULL)
	, m_dwWorkingPaletteSize(0)
	, m_pmWorkingPalette(NULL)
	, m_pEffect(NULL)
	, m_vMin(0, 0, 0) /// >> : OBB
	, m_vMax(0, 0, 0) /// >> : OBB
{
}

cSkinnedMesh::~cSkinnedMesh(void)
{
	for each(auto p in m_vecAnimSet)
		SAFE_RELEASE(p);

	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Load(char* szDirectory, char* szFilename)
{
	m_pEffect = LoadEffect("MultiAnimation.hpp");

	int nPaletteSize = 0;
	m_pEffect->GetInt("MATRIX_PALETTE_SIZE", &nPaletteSize);

	cAllocateHierarchy ah;
	ah.SetDirectory(szDirectory);
	ah.SetDefaultPaletteSize(nPaletteSize);

	std::string sFullPath(szDirectory);
	sFullPath += std::string(szFilename);

	D3DXLoadMeshHierarchyFromX(sFullPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&ah,
		NULL,
		(LPD3DXFRAME*)&m_pRootFrame,
		&m_pAnimController);

	/// >> : OBB
	{
		m_vMin = ah.GetMin();
		m_vMax = ah.GetMax();
	}

	if (m_pmWorkingPalette)
		delete[] m_pmWorkingPalette;

	m_dwWorkingPaletteSize = ah.GetMaxPaletteSize();
	m_pmWorkingPalette = new D3DXMATRIX[m_dwWorkingPaletteSize];
	if (m_pmWorkingPalette == NULL)
	{
		m_dwWorkingPaletteSize = 0;
	}

	if (m_pRootFrame)
		SetupBoneMatrixPtrs(m_pRootFrame);
}

void cSkinnedMesh::UpdateAndRender()
{
	if (m_pAnimController)
	{
		m_pAnimController->AdvanceTime(g_pTimeManager->GetElapsedTime(), NULL);
		Update();
	}

	if (m_pRootFrame)
	{
		D3DXMATRIXA16 matSRT;
		D3DXMatrixIdentity(&matSRT);
	
		{
			if (m_pmatParent)
				UpdateLocal(m_pRootFrame, m_pmatParent);
			else
				UpdateLocal(m_pRootFrame, &matSRT);
		}

		if (m_pmatParent)
			matSRT = m_matWorldTM * (*m_pmatParent);
		else
			matSRT = m_matWorldTM;
	
		Update(m_pRootFrame, &matSRT);
		Render(m_pRootFrame);
	}
}

void cSkinnedMesh::Render(ST_BONE* pBone)
{
	assert(pBone);

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		LPD3DXBONECOMBINATION pBoneCombos =
			(LPD3DXBONECOMBINATION)(pBoneMesh->pBufBoneCombos->GetBufferPointer());

		D3DXMATRIXA16 matViewProj, matView, matProj;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
		matViewProj = matView * matProj;

		D3DXMATRIXA16 mView, mInvView;
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &mView);
		D3DXMatrixInverse(&mInvView, 0, &mView);
		D3DXVECTOR3 vEye = D3DXVECTOR3(0, 0, 0);
		D3DXVec3TransformCoord(&vEye, &vEye, &mInvView);

		for (DWORD dwAttrib = 0; dwAttrib < pBoneMesh->dwNumAttrGroups; ++dwAttrib)
		{
			for (DWORD dwPalEntry = 0; dwPalEntry < pBoneMesh->dwNumPaletteEntries; ++dwPalEntry)
			{
				DWORD dwMatrixIndex = pBoneCombos[dwAttrib].BoneId[dwPalEntry];
				if (dwMatrixIndex != UINT_MAX)
				{
					m_pmWorkingPalette[dwPalEntry] =
						pBoneMesh->pBoneOffsetMatrices[dwMatrixIndex] *
						(*pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
				}
			}

			m_pEffect->SetMatrixArray("amPalette",
				m_pmWorkingPalette,
				pBoneMesh->dwNumPaletteEntries);

			m_pEffect->SetMatrix("g_mViewProj", &matViewProj);
			m_pEffect->SetVector("vLightDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetVector("vWorldLightPos", &D3DXVECTOR4(500.0f, 500.0f, 500.0f, 1.0f));
			m_pEffect->SetVector("vWorldCameraPos", &D3DXVECTOR4(vEye, 1.0f));
			m_pEffect->SetVector("vMaterialAmbient", &D3DXVECTOR4(0.53f, 0.53f, 0.53f, 0.53f));
			m_pEffect->SetVector("vMaterialDiffuse", &D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pEffect->SetTexture("g_txScene", pBoneMesh->vecTexture[pBoneCombos[dwAttrib].AttribId]);
			m_pEffect->SetInt("CurNumBones", pBoneMesh->dwMaxNumFaceInfls - 1);
			m_pEffect->SetTechnique("Skinning20");

			UINT uiPasses, uiPass;
			m_pEffect->Begin(&uiPasses, 0);
			for (uiPass = 0; uiPass < uiPasses; ++uiPass)
			{
				m_pEffect->BeginPass(uiPass);
				pBoneMesh->pWorkingMesh->DrawSubset(dwAttrib);
				m_pEffect->EndPass();
			}
			m_pEffect->End();
		}
	}

	if (pBone->pFrameSibling)
	{
		Render((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		Render((ST_BONE*)pBone->pFrameFirstChild);
	}
}

LPD3DXEFFECT cSkinnedMesh::LoadEffect(char* szFilename)
{
	LPD3DXEFFECT pEffect = NULL;

	D3DXMACRO mac[2] =
	{
		{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
		{ NULL,                          NULL }
	};

	D3DCAPS9 caps;
	D3DXMACRO *pmac = NULL;
	g_pD3DDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
		pmac = mac;

	DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
	dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
	dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

	ID3DXBuffer* pBuffer = NULL;
	if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice,
		szFilename,
		pmac,
		NULL,
		dwShaderFlags,
		NULL,
		&pEffect,
		&pBuffer)))
	{
		if (pBuffer)
		{
			OutputDebugStringA((char*)pBuffer->GetBufferPointer());
			SAFE_RELEASE(pBuffer);
		}

		return NULL;
	}

	return pEffect;
}

void cSkinnedMesh::Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent)
{
	pCurrent->CombinedTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->CombinedTransformationMatrix =
			pCurrent->CombinedTransformationMatrix * (*pmatParent);
	}

	if (pCurrent->pFrameSibling)
	{
		Update((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		Update((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->CombinedTransformationMatrix));
	}



}

void cSkinnedMesh::UpdateLocal(ST_BONE * pCurrent, D3DXMATRIXA16 * pmatParent)
{
	pCurrent->LocalTransformationMatrix = pCurrent->TransformationMatrix;
	if (pmatParent)
	{
		pCurrent->LocalTransformationMatrix =
			pCurrent->LocalTransformationMatrix * (*pmatParent);
	}

	if (pCurrent->pFrameSibling)
	{
		UpdateLocal((ST_BONE*)pCurrent->pFrameSibling, pmatParent);
	}

	if (pCurrent->pFrameFirstChild)
	{
		UpdateLocal((ST_BONE*)pCurrent->pFrameFirstChild, &(pCurrent->LocalTransformationMatrix));
	}

}

void cSkinnedMesh::SetupBoneMatrixPtrs(ST_BONE* pBone)
{
	assert(pBone);

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			LPD3DXSKININFO pSkinInfo = pBoneMesh->pSkinInfo;
			DWORD dwNumBones = pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; ++i)
			{
				LPCSTR szBoneName = pSkinInfo->GetBoneName(i);
				if (szBoneName == NULL || strlen(szBoneName) == 0)
					continue;
				ST_BONE* pInfluence = (ST_BONE*)D3DXFrameFind(m_pRootFrame, szBoneName);
				pBoneMesh->ppBoneMatrixPtrs[i] = &(pInfluence->CombinedTransformationMatrix);
			}
		}
	}
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameSibling);
	}

	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	if (!m_pAnimController)
		return;
	LPD3DXANIMATIONSET pAnimSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAnimSet);
	m_pAnimController->SetTrackAnimationSet(0, pAnimSet);
	//m_pAnimController->SetTrackPosition(0, 0.0f);

	SAFE_RELEASE(pAnimSet);

}

void cSkinnedMesh::Destroy()
{
	cAllocateHierarchy ah;
	D3DXFrameDestroy((LPD3DXFRAME)m_pRootFrame, &ah);
	SAFE_DELETE_ARRAY(m_pmWorkingPalette);
	SAFE_RELEASE(m_pEffect);
}

void cSkinnedMesh::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//�̹� �÷��̵ǰ� �ִ� Animation �̶�� ������ ���� �ʴ´�. ( ���� Animation �ߺ����� �ʰ� ���Ҵ� )
	if (this->m_pNowPlayAnimationSet != NULL &&
		animSet == this->m_pNowPlayAnimationSet)
		return;

	if (this->m_fCrossFadeTime > 0.0f)
	{

		//
		// ������ Animation �� 1 �� track ���� ����....
		//

		//���� Animation �� 1 ��Track ���� ����
		m_pAnimController->SetTrackAnimationSet(
			1, m_pNowPlayAnimationSet);

		//1�� Ʈ���� ��ġ�� ������ �÷��� �Ǵ� ��ġ�� ����
		m_pAnimController->SetTrackPosition(
			1,
			m_Track_Desc_0.Position);

		//1 �� Track Ȱ��ȭ
		m_pAnimController->SetTrackEnable(1, true);

		//1 �� Track ����ġ
		m_pAnimController->SetTrackWeight(1, 1.0f);

		//1 �� Track �� ���� 0 ���� Animation �ӵ��� ����.
		m_pAnimController->SetTrackSpeed(1,
			m_Track_Desc_0.Speed);


		//
		// ���ݼ��õǴ� Animation �� 0 ��Track ���� ����
		//

		//���ο� Animation 0 ������ ����
		m_pAnimController->SetTrackAnimationSet(0, animSet);
		//0�� Track Animation ��ġ 0.0 Ÿ�ӿ��� ���� ( ó������ �����ϰڴٴ¿��� )
		m_pAnimController->SetTrackPosition(0, 0.0f);
		//����ġ�� 0 ���� //����ġ�� 0 ���� 
		m_pAnimController->SetTrackWeight(0, 0.0f);

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->m_pNowPlayAnimationSet = animSet;
	}

	//ũ�ν� ���̵尡 ���ٸ�...
	else
	{

		//Track �� �������� �Ǿ����� ������.
		this->m_pAnimController->SetTrackPosition(
			0,			//Track ��ȣ 
			0.0			//PlayPosition
		);

		//�Ű������� ���� AnimationSet ���� Animation 0 �� Track�� �÷���
		this->m_pAnimController->SetTrackAnimationSet(0, animSet);

		//���� �÷��� �ǰ� �ִ� AnimationSet ����
		this->m_pNowPlayAnimationSet = animSet;

	}


}

void cSkinnedMesh::SetRandomTrackPosition()
{
	m_pAnimController->SetTrackPosition(0, (rand() % 100) / 10.0f);
}

void cSkinnedMesh::SetTransform(D3DXMATRIXA16 * pmat)
{
	m_matWorldTM = *pmat;
}

D3DXMATRIXA16 * cSkinnedMesh::GetTransForm()
{
	return &m_matWorldTM;
}
