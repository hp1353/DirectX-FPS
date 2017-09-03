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
		m_pRootFrame,			//루트 본전달
		name		//찾을 본이름 전달
	);

	if (pFrame != NULL)
		return (ST_BONE*)pFrame;

	return NULL;
}

void cSkinnedMesh::Update()
{
	m_fPassedTime += g_pTimeManager->GetElapsedTime();
	//0 번 Track 정보를 얻는다( 매프레임마다 갱신해야 한다 )
	m_pAnimController->GetTrackDesc(0, &m_Track_Desc_0);

	//m_pNowPlayAnimationSet->GetPeriod();		//현제 애니메이션의 전체 길이
	//m_Track_Desc_0.Position;					//Track 0 번에 플레이되고있는 Animation 플레이 Position

	//현재 Animation Play 기간을 0 ~ 1 로 봤을때 진행도....
	//주의 사항 루프시 ( 1 이 넘어감 )
	m_AnimationPlayFactor = m_Track_Desc_0.Position / m_pNowPlayAnimationSet->GetPeriod();


	//Loop 가 안되는 Animation 이 한번 플레이되고 끝나면...
	if (m_AnimationPlayFactor >= 0.99 && this->m_bLoop == false)
	{
		m_fPassedTime = 0.f;
		//트랙을 마지막으로 고정
		m_pAnimController->SetTrackPosition(0, m_Track_Desc_0.Priority);

		//돌아갈 Animation 이 있다면..
		if (this->m_pPrevPlayAnimationSet != NULL)
		{
			m_fCrossFadeTime = m_fOutCrossFadeTime;
			m_fLeftCrossFadeTime = m_fOutCrossFadeTime;
			m_bLoop = true;
			this->SetAnimation(m_pPrevPlayAnimationSet);
			this->m_pPrevPlayAnimationSet = NULL;
		}

		//돌아갈 Animation 이 없다면.
		else
		{
			this->m_bPlay = false;
		}

	}




	if (m_bPlay) {

		//인스턴싱된 SkinnedAnimation 클레스에서 AdvanceTime 진행시켜버리면
		//하나밖에업는 본이 해당 SkinnedAnimation 인스턴스의 Animation 정보로 
		//업데이트 된다
		//따라서 인스턴싱된 SkinnedAnimation 은 업데이트 량을 기억해놓았다가
		//Rendering 될때 AdvanceTime 각각 진행시킨다.
		m_fAnimDelta = m_fPassedTime;
	}
	else {
		m_fAnimDelta = 0.0f;
		m_fPassedTime = 0.f;
	}



	//크로스 페이드가 진행중이라면..
	if (m_fLeftCrossFadeTime > 0.0f)	//남은 크로스페이드 타임이 존재하는가 Play 할때 셋팅이된다.
	{
		//남은 크로스페이드 시간 뺀다
		this->m_fLeftCrossFadeTime -= m_fPassedTime;

		//크로스페이드 가끝났다.
		if (m_fLeftCrossFadeTime <= 0.0f)
		{
			//블렌딩 끝
			m_fLeftCrossFadeTime = 0.0f;
			m_pAnimController->SetTrackWeight(0, 1);		//0 번 Track 가중치 1
			m_pAnimController->SetTrackEnable(1, false);	//1 번 Track 비활성화
			
			m_fPassedTime = 0.f;
		}

		else
		{
			float w1 = (m_fLeftCrossFadeTime / m_fCrossFadeTime);		//1번 Track 가중치
			float w0 = 1.0f - w1;										//0번 Track 가중치

																		//가중치 셋팅
			m_pAnimController->SetTrackWeight(0, w0);
			m_pAnimController->SetTrackWeight(1, w1);
		}
	}
}

void cSkinnedMesh::Play(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = true;

	//맵에서 가져와 처리
	std::map< std::string, LPD3DXANIMATIONSET>::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end()) {

		this->m_pPrevPlayAnimationSet = NULL;

		//크로스 페이드 타임 기억
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

	//벡터에서 가져와 처리
	if (animIndex < this->m_AnimNum) {

		this->m_pPrevPlayAnimationSet = NULL;

		//크로스 페이드 타임 기억
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
			//현재 Animaiton 을 기억한다.
			this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;
		}


		//크로스 페이드 타임 기억
		m_fCrossFadeTime = inCrossFadeTime;
		m_fLeftCrossFadeTime = inCrossFadeTime;

		//나갈때 크로스페이드 타입 기억
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
		//현재 Animaiton 을 기억한다.
		this->m_pPrevPlayAnimationSet = this->m_pNowPlayAnimationSet;
	}


	//크로스 페이드 타임 기억
	m_fCrossFadeTime = inCrossFadeTime;
	m_fLeftCrossFadeTime = inCrossFadeTime;

	//나갈때 크로스페이드 타입 기억
	m_fOutCrossFadeTime = outCrossFadeTime;

	this->SetAnimation(m_vecAnimSet[animIndex]);

}

void cSkinnedMesh::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	std::map< std::string, LPD3DXANIMATIONSET>::iterator find = this->m_mapAnimSet.find(animName);
	if (find != this->m_mapAnimSet.end()) {

		//돌아갈 Animation 은 존재하지 않는다.
		this->m_pPrevPlayAnimationSet = NULL;

		//크로스 페이드 타임 기억
		m_fCrossFadeTime = crossFadeTime;
		m_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void cSkinnedMesh::PlayOneShotAfterHold(int animIndex, float crossFadeTime)
{
	m_bPlay = true;
	m_bLoop = false;

	//돌아갈 Animation 은 존재하지 않는다.
	this->m_pPrevPlayAnimationSet = NULL;

	//크로스 페이드 타임 기억
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
	//이미 플레이되고 있는 Animation 이라면 실행이 되지 않는다. ( 같은 Animation 중복되지 않게 막았다 )
	if (this->m_pNowPlayAnimationSet != NULL &&
		animSet == this->m_pNowPlayAnimationSet)
		return;

	if (this->m_fCrossFadeTime > 0.0f)
	{

		//
		// 현재의 Animation 을 1 번 track 으로 전달....
		//

		//현제 Animation 을 1 번Track 으로 셋팅
		m_pAnimController->SetTrackAnimationSet(
			1, m_pNowPlayAnimationSet);

		//1번 트랙의 위치를 이전에 플레이 되던 위치로 셋팅
		m_pAnimController->SetTrackPosition(
			1,
			m_Track_Desc_0.Position);

		//1 번 Track 활성화
		m_pAnimController->SetTrackEnable(1, true);

		//1 번 Track 가중치
		m_pAnimController->SetTrackWeight(1, 1.0f);

		//1 번 Track 에 기존 0 번의 Animation 속도도 전달.
		m_pAnimController->SetTrackSpeed(1,
			m_Track_Desc_0.Speed);


		//
		// 지금셋팅되는 Animation 을 0 번Track 으로 셋팅
		//

		//새로운 Animation 0 번으로 셋팅
		m_pAnimController->SetTrackAnimationSet(0, animSet);
		//0번 Track Animation 위치 0.0 타임에서 시작 ( 처음부터 시작하겠다는예기 )
		m_pAnimController->SetTrackPosition(0, 0.0f);
		//가중치는 0 으로 //가중치는 0 으로 
		m_pAnimController->SetTrackWeight(0, 0.0f);

		//현재 플레이 되고 있는 AnimationSet 갱신
		this->m_pNowPlayAnimationSet = animSet;
	}

	//크로스 페이드가 없다면...
	else
	{

		//Track 의 포지션을 맨압으로 돌린다.
		this->m_pAnimController->SetTrackPosition(
			0,			//Track 번호 
			0.0			//PlayPosition
		);

		//매개변수로 받은 AnimationSet 으로 Animation 0 번 Track에 플레이
		this->m_pAnimController->SetTrackAnimationSet(0, animSet);

		//현재 플레이 되고 있는 AnimationSet 갱신
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
