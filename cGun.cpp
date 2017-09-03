#include "stdafx.h"
#include "cGun.h"
#include "cSkinnedMesh.h"
#include "cBullet.h"
#include "cEffect.h"

cGun::cGun()
	: m_pGun(NULL)
	, m_pvTarget(NULL)
	, m_fAttackPower(1.f)
	, m_fAttackRange(10.f)
	, m_fAttackSpeed(1.f)
	, m_nMaxBullet(100)
	, m_nCurrentBullet(8)
	, m_nMagazine(8)
	, m_fCurrentExp(0.f)
	, m_fMaxExp(7.f)
	, m_CurrentLv(0)
	, m_MaxLv(3)
{
	D3DXMatrixIdentity(&m_pTrans);
}


cGun::~cGun()
{
	SAFE_RELEASE(m_pMesh);

	for each (auto p in m_pvEffect)
		SAFE_RELEASE(p);

	for each (auto p in m_pvBullet)
		SAFE_RELEASE(p);

	SAFE_DELETE(m_pGun);

	m_pvEffect.clear();
	m_pvBullet.clear();
}

void cGun::Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename)
{
	m_pvTarget = pvTarget;

	m_pGun = new cSkinnedMesh(szFolder, szFilename);


	D3DXCreateSphere(g_pD3DDevice, 0.1, 20, 20, &m_pMesh, NULL);
	
}

void cGun::Update()
{
	for (size_t i = 0; i < m_pvBullet.size(); )
	{
		m_pvBullet[i]->Update();
		if(m_pvBullet[i]->GetMoveDistance() > m_fAttackRange)
		{
			SAFE_RELEASE(m_pvBullet[i]);
			m_pvBullet.erase(m_pvBullet.begin());
			continue;
		}
		i++;
	}

	for (size_t i = 0; i < m_pvEffect.size(); )
	{
		if (!m_pvEffect[i]->GetPlay())
		{
			m_pvEffect[i]->Destroy();
			m_pvEffect.erase(m_pvEffect.begin() + i);
			continue;
		}

		m_pvEffect[i]->Update();
		i++;
	}

}

void cGun::Render()
{
	if (m_pGun)
		m_pGun->UpdateAndRender();
	
	if (*m_pGun->getLocalMatrix("tag_barrell") != NULL)
	{
		m_matTagBarrel = *m_pGun->getLocalMatrix("tag_barrell") * *m_pGun->GetParent();

		/*D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		matWorld._41 = m_matTagBarrel._41;
		matWorld._42 = m_matTagBarrel._42;
		matWorld._43 = m_matTagBarrel._43;

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

		m_pMesh->DrawSubset(0);*/
	}
	

	for each (auto p in m_pvEffect)
		p->Render();

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);

	for (size_t i = 0; i < m_pvBullet.size(); i++)
	{
		m_pvBullet[i]->Render();
	}



}

void cGun::SetParentWorldMatrix(D3DXMATRIXA16* matWorld)
{
	if (m_pGun)
		m_pGun->SetParent(matWorld);
}

void cGun::SetWorldMatrix(D3DXMATRIXA16* matWorld)
{
	if (m_pGun)
		m_pGun->SetTransform(matWorld);
}
void cGun::SetWorldMatrixByBoneName(D3DXMATRIXA16 * matRot, char * name)
{
	if (m_pGun)
	{
		D3DXMATRIXA16 matWorld = *matRot;
		m_pTrans = *m_pGun->getWorldMatrix(name);
		D3DXMatrixInverse(&m_pTransInv, 0, &m_pTrans);
		m_pWorldTM = m_pTrans * matWorld * m_pTransInv * *m_pGun->getWorldMatrix(name);
		m_pGun->SetTransform(&m_pWorldTM);
	}
}

bool cGun::Fire(D3DXVECTOR3 vDirection, D3DXMATRIXA16 & matWorld)
{
	if (m_nCurrentBullet <= 0)
	{
		Reload();

		// 총알 발사는 안할거니까 false 리턴
		return false;
	}

	m_nCurrentBullet--;

	D3DXMATRIXA16 matPos = matWorld;

	// 총구에서 나가도록 설정
	if (*m_pGun->getLocalMatrix("tip") != NULL)
	{
		matPos = *m_pGun->getLocalMatrix("tip") * matPos;
	}
	else if (*m_pGun->getLocalMatrix("tag_barrell") != NULL)
	{
		matPos = m_matTagBarrel; 
	}
	
	D3DXVECTOR3 vecPos = D3DXVECTOR3(matPos._41, matPos._42, matPos._43);

	if (m_pvBullet.size() < m_nMaxBullet)
	{
		cBullet* bullet = new cBullet;
		bullet->Setup(vDirection, vecPos);
		m_pvBullet.push_back(bullet);
	}

	cEffect* effect = new cEffect;
	effect->Setup("Effect/test4.tga", 1, 1, 2, 1);
	effect->SetPosition(D3DXVECTOR3(vecPos.x, vecPos.y, vecPos.z));
	m_pvEffect.push_back(effect);

	return true;
}

void cGun::Reload()
{
	// 총알 채워준다.  외부에서 재장전 애니메이션 실행함.
	if (m_nMaxBullet >= m_nMagazine)
	{
		// currentbullet이 남아있는채로 reload하는 경우 더해줌..
		m_nMaxBullet -= (m_nMagazine - m_nCurrentBullet);
		m_nCurrentBullet = m_nMagazine;
	}
	else
	{
		m_nCurrentBullet = m_nMaxBullet;
		m_nMaxBullet = 0;
	}

}

void cGun::RemoveBullet(int bulletIndex)
{
	m_pvBullet.erase(m_pvBullet.begin() + bulletIndex);
}

bool cGun::IsShoot()
{
	return m_pGun->IsPlay("shot");
}

bool cGun::IsReload()
{
	return m_pGun->IsPlay("reload");
}

cSkinnedMesh * cGun::GetGunMesh()
{
	return m_pGun;
}

