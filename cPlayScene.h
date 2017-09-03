#pragma once

#include "cScene.h"
#include "cUIButton.h"

class cCamera;
class cTextMap;
class cHeightMap;

class cUIObject;
class cUIImageView;
class cStaticMesh;

class cPlayer;
class cAI;

class cBuildingGroup;
class cEvent;

class cAirDrop;
class cFrustum;
class cPlane;



class cLight;



enum PLAY_STATE
{
	AIRDROP_STATE,
	NORMAL_STATE,
	PAUSE_STATE,
	GAME_OVER,
	GAME_CLEAR
};

enum QUIT_BUTTON_STATE
{
	EXIT_BTN,
	RETURN_BTN
};

class cPlayScene 
	: public cScene
	, public iButtonDelegate
{
	cCamera*					m_pCamera;

	cStaticMesh*				m_pSkyView;
	cTextMap*					m_pTextMap;
	cHeightMap*					m_pHeightMap;

	cPlayer*					m_pPlayer;
	std::vector<cAI*>			m_pvAI;
	std::vector<cAI*>			m_pvDeathAI;

	std::vector<cBuildingGroup*> m_pvBuildingGroup;

	cEvent*						m_pEvent;
	std::vector<cEvent*>		m_pAttackEvent;
	
	PLAY_STATE					m_eState;
	PLAY_STATE					m_ePrevState;
	cAirDrop*					m_pAirDrop;
	cUIObject*                  m_pQuitGameRoot;
	cUIObject*					m_pQuestInfoRoot;

	cUIImageView*	m_pCurEnemyNumFirstPos;
	cUIImageView*	m_pCurEnemyNumSecondPos;

	LPD3DXSPRITE		        m_pSprite;

	cFrustum*					m_pFrustum;
	
	std::vector<cPlane*>        m_pvPlane;

	int                         m_FireCount;
	int                         m_HitCount;
	float                       m_startTime;
	float                       m_endTime;

	int							m_nAIMaxSize;

public:
	cPlayScene();
	virtual ~cPlayScene();

	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void PlayerBulletCollision();
	void AIBulletCollision();

	void SettingBuildingGroup();
	void UpdatePlayerBuildingGroup();

	float GetDistance(D3DXVECTOR3 BulletPos, D3DXVECTOR3 CrushManPos);
	bool IsCollision(D3DXVECTOR3 BulletPos, float BulletSphereRadius, D3DXVECTOR3 CrushManPos, float CrushManSphereRadius);
	void LevUpCheck();

	void QuitGameUISetting();
	void MissionUISetting();
	void MissionUIUpdate();
	void OnClick(cUIButton * pSender);
	void SaveAccuracyRate();
	void SavePlayTime();
};

