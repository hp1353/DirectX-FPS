#pragma once
#include "cScene.h"
#include "cUIButton.h"
#include "cUIImageView.h"

class cUIObject;

enum MAIN_MENU_STATE {
	MAIN_MENU,
	OPTION_MENU,
	SOUND_MENU,
	MEDAL_MENU
};

enum MAIN_MENU_BUTTON {
	CAMPAIN_BTN,
	OPTION_BTN,
	QUIT_BTN,
	AUDIO_BTN,
	MAP_BTN,
	BACK_BTN,
	MEDAL_BTN,
	FIRST_MEDAL_BTN,
	SECOND_MEDAL_BTN,
	THIRD_MEDAL_BTN,
	FOURTH_MEDAL_BTN,
	FIFTH_MEDAL_BTN,
	SIXTH_MEDAL_BTN,
	FAKE_BTN
};

class cFirstScene : public cScene
	, public iButtonDelegate
{
	cUIObject*			m_pMainUIRoot;
	cUIObject*			m_pOptionUIRoot;
	cUIObject*			m_pSoundUIRoot;
	cUIObject*          m_pMedalUIRoot;
	LPD3DXSPRITE		m_pSprite;

	MAIN_MENU_STATE		m_stMenu;
	POINT               m_ptMouse;
	bool                m_bIsLButtonDown;
	RECT                m_rSliderRect;
	cUIImageView *      m_pSoundSlideBar;
	cUIImageView *      m_pSoundSlider;

	cUIImageView*       m_pNinemmLv1;
	cUIImageView*       m_pNinemmLv2;
	cUIImageView*       m_pNinemmLv3;

	cUIImageView*       m_pShotgunLv1;
	cUIImageView*       m_pShotgunLv2;
	cUIImageView*       m_pShotgunLv3;

	cUIImageView*       m_pWincLv1;
	cUIImageView*       m_pWincLv2;
	cUIImageView*       m_pWincLv3;

	bool                m_honer[6];

	cUIImageView*       m_pSoldierHonerText;
	cUIImageView*       m_pTakeConditionText;

	cUIButton*          m_pBtnAudio;
	cUIButton*          m_pBtnMedal;
	cUIButton*          m_pBtnBack;

public:
	cFirstScene();
	virtual ~cFirstScene();

	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnClick(cUIButton * pSender);

	void MainMenuUISetting();
	void OptionMenuUISetting();
	void SoundMenuSetting();
	void MedalMenuSetting();
	void SoundBarControl();
	void LoadHoner(char * szFilename);
	void LoadAccuracyRate(char * szFilename);
	void LoadHeathRate(char * szFileName);
	void LoadPlayTime(char * szFilename);
};

