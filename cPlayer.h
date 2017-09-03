#pragma once

#define GUNMAX 3

#include "cCharacter.h"
class cRay;
class cUIObject;
class cUIImageView;
class cUITextView;
class cBuildingGroup;

enum GUN_MODE
{
	GUN_NORMAL_MODE,
	GUN_ZOOM_MODE
};

class cPlayer : public cCharacter
{
	std::vector<cGun*>	m_vecGun;
	int					m_nSelectGun;
	cRay*				m_pBulletRay;

	D3DXVECTOR3			m_vCamAngle;

	cOBB*               m_pPlayerOBB;

	ST_SPHERE			m_stSphere;
	
	LPD3DXSPRITE		m_pSprite;
	cUIObject*			m_pUICursorRoot;
	cUIObject*			m_pUIGunInfoRoot;
	cUIObject*			m_pUIPlayerInfoRoot;

	cUIImageView*		m_pImageCursorL;
	cUIImageView*		m_pImageCursorR;
	cUIImageView*		m_pImageCursorT;
	cUIImageView*		m_pImageCursorB;

	cUIImageView*		m_pImageGunFront;
	cUIImageView*		m_pImageGunBack;
	cUITextView*		m_pBulletText;

	cUIImageView*		m_pCompassFront;
	cUIImageView*		m_pUIPlayerHP;

	float				m_fActionPower;
	bool				m_isAction;
	bool				m_isActionUp;
	
	char*               m_cFireGunSoundName;
	char*               m_cReloadGunSoundName;

	SYNTHESIZE(GUN_MODE, m_eGunMode, GunMode);
	SYNTHESIZE(cBuildingGroup*, m_pBuildings, Buildings);

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update(D3DXVECTOR3& camAngle);
	void Render();

	void SetHeightMap(cHeightMap* hMap);
	void SetTextMap(cTextMap* tMap);

	void GunChange();
	void GunSetting(D3DXVECTOR3& camAngle);

	void BulletFire(D3DXVECTOR3 dir);

	cRay* GetBullet();
	void DeleteRay();

	void SettingCursorUI();
	void SettingGunUI();
	void SettingPlayerInfoUI();
	void CursorAction(float ActionPower);

	void UIUpdate(float angle);
	void UIRender();

	D3DXVECTOR3 GetSphereCenter();
};

