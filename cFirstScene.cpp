#include "stdafx.h"
#include "cFirstScene.h"
#include "cUIImageView.h"


cFirstScene::cFirstScene()
	: m_pMainUIRoot(NULL)
	, m_pOptionUIRoot(NULL)
	, m_pSprite(NULL)
	, m_pMedalUIRoot(NULL)
	, m_stMenu(MAIN_MENU)
{
}


cFirstScene::~cFirstScene()
{

}

void cFirstScene::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	LoadHoner("Guntext.txt");
	LoadHeathRate("HeathRate.txt");
	LoadAccuracyRate("AccuracyRate.txt");
	LoadPlayTime("PlayTime.txt");

	MainMenuUISetting();
	OptionMenuUISetting();
	SoundMenuSetting();

	MedalMenuSetting();
}

void cFirstScene::Destroy()
{
	if (m_pMainUIRoot)
		m_pMainUIRoot->Destroy();

	if (m_pOptionUIRoot)
		m_pOptionUIRoot->Destroy();

	if (m_pSoundUIRoot)
		m_pSoundUIRoot->Destroy();

	if (m_pMedalUIRoot)
		m_pMedalUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);
}

void cFirstScene::Update()
{
	switch (m_stMenu)
	{
	case MAIN_MENU:
		if (m_pMainUIRoot)
			m_pMainUIRoot->Update();
		break;
	case SOUND_MENU:
		if (m_pSoundUIRoot)
			m_pSoundUIRoot->Update();
	case OPTION_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Update();
		break;
	case MEDAL_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Update();
		if (m_pMedalUIRoot)
			m_pMedalUIRoot->Update();
		break;
	default:
		break;
	}



	SoundBarControl();


}

void cFirstScene::Render()
{
	switch (m_stMenu)
	{
	case MAIN_MENU:
		if (m_pMainUIRoot)
			m_pMainUIRoot->Render(m_pSprite);
		break;

	case OPTION_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Render(m_pSprite);
		m_pBtnAudio->SetTexture("MainMenuUI/audio_black.png", "MainMenuUI/audio_white.png", "MainMenuUI/audio_white.png");
		m_pBtnMedal->SetTexture("MainMenuUI/medal_black.png", "MainMenuUI/medal_white.png", "MainMenuUI/medal_white.png");
		m_pBtnBack->SetTexture("MainMenuUI/back_black.png", "MainMenuUI/back_white.png", "MainMenuUI/back_white.png");
		m_pBtnAudio->SetTag(AUDIO_BTN);
		m_pBtnMedal->SetTag(MEDAL_BTN);
		m_pBtnBack->SetTag(BACK_BTN);

		break;
	case SOUND_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Render(m_pSprite);
		if (m_pSoundUIRoot)
			m_pSoundUIRoot->Render(m_pSprite);
		break;
	case MEDAL_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Render(m_pSprite);
		if (m_pMedalUIRoot)
			m_pMedalUIRoot->Render(m_pSprite);
		m_pBtnAudio->SetTexture("MainMenuUI/audio_black.png", "MainMenuUI/audio_black.png", "MainMenuUI/audio_black.png");
		m_pBtnMedal->SetTexture("MainMenuUI/medal_black.png", "MainMenuUI/medal_black.png", "MainMenuUI/medal_black.png");
		m_pBtnBack->SetTexture("MainMenuUI/back_black.png", "MainMenuUI/back_black.png", "MainMenuUI/back_black.png");
		m_pBtnAudio->SetTag(FAKE_BTN);
		m_pBtnMedal->SetTag(FAKE_BTN);
		m_pBtnBack->SetTag(FAKE_BTN);
		break;
	default:
		break;
	}
}

void cFirstScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);
		m_bIsLButtonDown = true;
		break;
	case WM_LBUTTONUP:
		m_bIsLButtonDown = false;
	case WM_MOUSEMOVE:
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);

	}
}

void cFirstScene::OnClick(cUIButton * pSender)
{
	switch (pSender->GetTag())
	{
	case CAMPAIN_BTN:
		g_pSceneManager->ChangeSceneWithLoading("playScene", "loadingScene");
		break;
	case OPTION_BTN:
		m_stMenu = OPTION_MENU;
		break;
	case QUIT_BTN:
		exit(0);
		break;
	case AUDIO_BTN:

		if (m_stMenu == SOUND_MENU)
			m_stMenu = OPTION_MENU;
		else
			m_stMenu = SOUND_MENU;

		break;
	case MEDAL_BTN:
		m_stMenu = MEDAL_MENU;
		break;
	case BACK_BTN:
		if (m_stMenu == MEDAL_MENU) m_stMenu = OPTION_MENU;
		else m_stMenu = MAIN_MENU;
		break;
	case FIRST_MEDAL_BTN:
		m_pSoldierHonerText->SetTexture("MainMenuUI/Medal/first_medal_reason.png");
		m_pTakeConditionText->SetTexture("MainMenuUI/Medal/first_medal_condition.png");
		break;
	case SECOND_MEDAL_BTN:
		m_pSoldierHonerText->SetTexture("MainMenuUI/Medal/second_medal_reason.png");
		m_pTakeConditionText->SetTexture("MainMenuUI/Medal/second_medal_condition.png");
		break;
	case THIRD_MEDAL_BTN:
		m_pSoldierHonerText->SetTexture("MainMenuUI/Medal/third_medal_reason.png");
		m_pTakeConditionText->SetTexture("MainMenuUI/Medal/third_medal_condition.png");
		break;
	case FOURTH_MEDAL_BTN:
		m_pSoldierHonerText->SetTexture("MainMenuUI/Medal/fourth_medal_reason.png");
		m_pTakeConditionText->SetTexture("MainMenuUI/Medal/fourth_medal_condition.png");
		break;
	case FIFTH_MEDAL_BTN:
		m_pSoldierHonerText->SetTexture("MainMenuUI/Medal/fifth_medal_reason.png");
		m_pTakeConditionText->SetTexture("MainMenuUI/Medal/fifth_medal_condition.png");
		break;
	case SIXTH_MEDAL_BTN:
		m_pSoldierHonerText->SetTexture("MainMenuUI/Medal/sixth_medal_reason.png");
		m_pTakeConditionText->SetTexture("MainMenuUI/Medal/sixth_medal_condition.png");
		break;
	}



	if (pSender->GetTag() != FAKE_BTN)
		g_pSoundManager->play("MenuSelect", 0.5f * g_pSoundManager->GetSoundVol());
}

void cFirstScene::MainMenuUISetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pMainUIRoot = new cUIObject;
	m_pMainUIRoot->SetPosition(0, 0);

	cUIImageView* MainMenuBackground = new cUIImageView;
	MainMenuBackground->SetTexture("MainMenuUI/main_1.png");
	MainMenuBackground->SetPosition(MainMenuBackground->GetSize().nWidth / 2, MainMenuBackground->GetSize().nHeight / 2);

	cUIImageView* MainMenuImg = new cUIImageView;
	MainMenuImg->SetTexture("MainMenuUI/main_menu.png");
	MainMenuImg->SetPosition(rc.left + MainMenuImg->GetSize().nWidth / 2 + 30, rc.top + 50);

	cUIButton* BtnCampain = new cUIButton;
	BtnCampain->SetTexture("MainMenuUI/campain_black.png", "MainMenuUI/campain_white.png", "MainMenuUI/campain_white.png");
	BtnCampain->SetTag(CAMPAIN_BTN);
	BtnCampain->SetPosition(rc.right - BtnCampain->GetSize().nWidth - 50, rc.top + 250);
	BtnCampain->SetDelegate(this);

	cUIButton* BtnOption = new cUIButton;
	BtnOption->SetTexture("MainMenuUI/option_black.png", "MainMenuUI/option_white.png", "MainMenuUI/option_white.png");
	BtnOption->SetTag(OPTION_BTN);
	BtnOption->SetPosition(rc.right - BtnOption->GetSize().nWidth - 50, rc.top + 300);
	BtnOption->SetDelegate(this);

	cUIButton* BtnQuit = new cUIButton;
	BtnQuit->SetTexture("MainMenuUI/quit_black.png", "MainMenuUI/quit_white.png", "MainMenuUI/quit_white.png");
	BtnQuit->SetTag(QUIT_BTN);
	BtnQuit->SetPosition(rc.right - BtnQuit->GetSize().nWidth - 50, rc.top + 350);
	BtnQuit->SetDelegate(this);

	m_pMainUIRoot->AddChild(MainMenuBackground);
	m_pMainUIRoot->AddChild(MainMenuImg);
	m_pMainUIRoot->AddChild(BtnCampain);
	m_pMainUIRoot->AddChild(BtnOption);
	m_pMainUIRoot->AddChild(BtnQuit);

}

void cFirstScene::OptionMenuUISetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pOptionUIRoot = new cUIObject;
	m_pOptionUIRoot->SetPosition(0, 0);

	cUIImageView* OptionBackground = new cUIImageView;
	OptionBackground->SetTexture("MainMenuUI/main_2.png");
	OptionBackground->SetPosition(OptionBackground->GetSize().nWidth / 2, OptionBackground->GetSize().nHeight / 2);

	cUIImageView* OptionImg = new cUIImageView;
	OptionImg->SetTexture("MainMenuUI/option_white.png");
	OptionImg->SetPosition(rc.left + OptionImg->GetSize().nWidth / 2 + 30, rc.top + 50);

	m_pBtnAudio = new cUIButton;
	m_pBtnAudio->SetTexture("MainMenuUI/audio_black.png", "MainMenuUI/audio_white.png", "MainMenuUI/audio_white.png");
	m_pBtnAudio->SetTag(AUDIO_BTN);
	m_pBtnAudio->SetPosition(rc.left + 30, 100);
	m_pBtnAudio->SetDelegate(this);

	m_pBtnMedal = new cUIButton;
	m_pBtnMedal->SetTexture("MainMenuUI/medal_black.png", "MainMenuUI/medal_white.png", "MainMenuUI/medal_white.png");
	m_pBtnMedal->SetTag(MEDAL_BTN);
	m_pBtnMedal->SetPosition(rc.left + 30, 150);
	m_pBtnMedal->SetDelegate(this);

	m_pBtnBack = new cUIButton;
	m_pBtnBack->SetTexture("MainMenuUI/back_black.png", "MainMenuUI/back_white.png", "MainMenuUI/back_white.png");
	m_pBtnBack->SetTag(BACK_BTN);
	m_pBtnBack->SetPosition(rc.left + 30, 200);
	m_pBtnBack->SetDelegate(this);



	m_pOptionUIRoot->AddChild(OptionBackground);
	m_pOptionUIRoot->AddChild(OptionImg);
	m_pOptionUIRoot->AddChild(m_pBtnAudio);
	m_pOptionUIRoot->AddChild(m_pBtnMedal);
	m_pOptionUIRoot->AddChild(m_pBtnBack);



}

void cFirstScene::SoundMenuSetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pSoundUIRoot = new cUIObject;
	m_pSoundUIRoot->SetPosition(0, 0);


	m_pSoundSlider = new cUIImageView;
	m_pSoundSlider->SetTexture("MainMenuUI/VolCast_off.png");
	m_pSoundSlider->SetPosition(rc.right - 200, (rc.top + rc.bottom) / 2);

	m_pSoundSlideBar = new cUIImageView;
	m_pSoundSlideBar->SetTexture("MainMenuUI/VolScaleButton_off.png");
	m_pSoundSlideBar->SetPosition(rc.right - 200, (rc.top + rc.bottom) / 2);


	m_rSliderRect = { (LONG)(m_pSoundSlider->GetPosition().x - m_pSoundSlider->GetSize().nWidth / 2 + 15),
		(LONG)(m_pSoundSlider->GetPosition().y - m_pSoundSlider->GetSize().nHeight / 2),
		(LONG)(m_pSoundSlider->GetPosition().x + m_pSoundSlider->GetSize().nWidth - 190) ,
		(LONG)(m_pSoundSlider->GetPosition().y + m_pSoundSlider->GetSize().nHeight) };

	m_pSoundUIRoot->AddChild(m_pSoundSlideBar);
	m_pSoundUIRoot->AddChild(m_pSoundSlider);
}

void cFirstScene::MedalMenuSetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pMedalUIRoot = new cUIObject;
	m_pMedalUIRoot->SetPosition(0, 0);

	cUIImageView* board = new cUIImageView;
	board->SetTexture("MainMenuUI/Medal/board.png");
	board->SetPosition(rc.left + 550, rc.top + 350);
	m_pMedalUIRoot->AddChild(board);

	if (m_honer[0])
	{
		cUIButton* FirstMedalOn = new cUIButton;
		FirstMedalOn->SetTexture("MainMenuUI/Medal/first_medal_unclick_possession.png",
			"MainMenuUI/Medal/first_medal_click_possession.png",
			"MainMenuUI/Medal/first_medal_click_possession.png");
		FirstMedalOn->SetPosition(rc.right - 780, rc.top + 130);
		FirstMedalOn->SetDelegate(this);
		FirstMedalOn->SetTag(FIRST_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(FirstMedalOn);
	}
	if (!m_honer[0])
	{
		cUIButton* FirstMedalOff = new cUIButton;
		FirstMedalOff->SetTexture("MainMenuUI/Medal/first_medal_unclick_non_possession.png",
			"MainMenuUI/Medal/first_medal_click_non_possession.png",
			"MainMenuUI/Medal/first_medal_click_non_possession.png");
		FirstMedalOff->SetPosition(rc.right - 780, rc.top + 130);
		FirstMedalOff->SetDelegate(this);
		FirstMedalOff->SetTag(FIRST_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(FirstMedalOff);
	}

	if (m_honer[1])
	{
		cUIButton* SecondMedalOn = new cUIButton;
		SecondMedalOn->SetTexture("MainMenuUI/Medal/second_medal_unclick_possession.png",
			"MainMenuUI/Medal/second_medal_click_possession.png",
			"MainMenuUI/Medal/second_medal_click_possession.png");
		SecondMedalOn->SetPosition(rc.right - 680, rc.top + 130);
		SecondMedalOn->SetDelegate(this);
		SecondMedalOn->SetTag(SECOND_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(SecondMedalOn);
	}
	if (!m_honer[1])
	{
		cUIButton* SecondMedalOff = new cUIButton;
		SecondMedalOff->SetTexture("MainMenuUI/Medal/second_medal_unclick_non_possession.png",
			"MainMenuUI/Medal/second_medal_click_non_possession.png",
			"MainMenuUI/Medal/second_medal_click_non_possession.png");
		SecondMedalOff->SetPosition(rc.right - 680, rc.top + 130);
		SecondMedalOff->SetDelegate(this);
		SecondMedalOff->SetTag(SECOND_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(SecondMedalOff);
	}
	if (m_honer[2])
	{
		cUIButton* ThirdMedalOn = new cUIButton;
		ThirdMedalOn->SetTexture("MainMenuUI/Medal/third_medal_unclick_possession.png",
			"MainMenuUI/Medal/third_medal_click_possession.png",
			"MainMenuUI/Medal/third_medal_click_possession.png");
		ThirdMedalOn->SetPosition(rc.right - 580, rc.top + 130);
		ThirdMedalOn->SetDelegate(this);
		ThirdMedalOn->SetTag(THIRD_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(ThirdMedalOn);
	}
	if (!m_honer[2])
	{
		cUIButton* ThirdMedalOff = new cUIButton;
		ThirdMedalOff->SetTexture("MainMenuUI/Medal/third_medal_unclick_non_possession.png",
			"MainMenuUI/Medal/third_medal_click_non_possession.png",
			"MainMenuUI/Medal/third_medal_click_non_possession.png");
		ThirdMedalOff->SetPosition(rc.right - 580, rc.top + 130);
		ThirdMedalOff->SetDelegate(this);
		ThirdMedalOff->SetTag(THIRD_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(ThirdMedalOff);
	}
	if (m_honer[3])
	{
		cUIButton* FourthMedalOn = new cUIButton;
		FourthMedalOn->SetTexture("MainMenuUI/Medal/fourth_medal_unclick_possession.png",
			"MainMenuUI/Medal/fourth_medal_click_possession.png",
			"MainMenuUI/Medal/fourth_medal_click_possession.png");
		FourthMedalOn->SetPosition(rc.right - 480, rc.top + 130);
		FourthMedalOn->SetDelegate(this);
		FourthMedalOn->SetTag(FOURTH_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(FourthMedalOn);
	}
	if (!m_honer[3])
	{
		cUIButton* FourthMedalOff = new cUIButton;
		FourthMedalOff->SetTexture("MainMenuUI/Medal/fourth_medal_unclick_non_possession.png",
			"MainMenuUI/Medal/fourth_medal_click_non_possession.png",
			"MainMenuUI/Medal/fourth_medal_click_non_possession.png");
		FourthMedalOff->SetPosition(rc.right - 480, rc.top + 130);
		FourthMedalOff->SetDelegate(this);
		FourthMedalOff->SetTag(FOURTH_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(FourthMedalOff);
	}
	if (m_honer[4])
	{
		cUIButton* FifthMedalOn = new cUIButton;
		FifthMedalOn->SetTexture("MainMenuUI/Medal/fifth_medal_unclick_possession.png",
			"MainMenuUI/Medal/fifth_medal_click_possession.png",
			"MainMenuUI/Medal/fifth_medal_click_possession.png");
		FifthMedalOn->SetPosition(rc.right - 380, rc.top + 130);
		FifthMedalOn->SetDelegate(this);
		FifthMedalOn->SetTag(FIFTH_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(FifthMedalOn);
	}
	if (!m_honer[4])
	{
		cUIButton* FifthMedalOff = new cUIButton;
		FifthMedalOff->SetTexture("MainMenuUI/Medal/fifth_medal_unclick_non_possession.png",
			"MainMenuUI/Medal/fifth_medal_click_non_possession.png",
			"MainMenuUI/Medal/fifth_medal_click_non_possession.png");
		FifthMedalOff->SetPosition(rc.right - 380, rc.top + 130);
		FifthMedalOff->SetDelegate(this);
		FifthMedalOff->SetTag(FIFTH_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(FifthMedalOff);
	}
	if (m_honer[5])
	{
		cUIButton* SixthMedalOn = new cUIButton;
		SixthMedalOn->SetTexture("MainMenuUI/Medal/sixth_medal_unclick_possession.png",
			"MainMenuUI/Medal/sixth_medal_click_possession.png",
			"MainMenuUI/Medal/sixth_medal_click_possession.png");
		SixthMedalOn->SetPosition(rc.right - 280, rc.top + 130);
		SixthMedalOn->SetDelegate(this);
		SixthMedalOn->SetTag(SIXTH_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(SixthMedalOn);
	}
	if (!m_honer[5])
	{
		cUIButton* SixthMedalOff = new cUIButton;
		SixthMedalOff->SetTexture("MainMenuUI/Medal/sixth_medal_unclick_non_possession.png",
			"MainMenuUI/Medal/sixth_medal_click_non_possession.png",
			"MainMenuUI/Medal/sixth_medal_click_non_possession.png");
		SixthMedalOff->SetPosition(rc.right - 280, rc.top + 130);
		SixthMedalOff->SetDelegate(this);
		SixthMedalOff->SetTag(SIXTH_MEDAL_BTN);
		m_pMedalUIRoot->AddChild(SixthMedalOff);
	}

	m_pSoldierHonerText = new cUIImageView; // ±ºÀÎÈÆÀå
	m_pSoldierHonerText->SetPosition(rc.left + 810, rc.top + 360);
	m_pMedalUIRoot->AddChild(m_pSoldierHonerText);

	m_pTakeConditionText = new cUIImageView; // È¹µæÁ¶°Ç
	m_pTakeConditionText->SetPosition(rc.left + 810, rc.top + 520);
	m_pMedalUIRoot->AddChild(m_pTakeConditionText);

	cUIButton* MedalMenuBack = new cUIButton;
	MedalMenuBack->SetPosition(rc.right - 180, rc.bottom - 50);
	MedalMenuBack->SetTexture("MainMenuUI/Medal/Back_Btn_off.png", "MainMenuUI/Medal/Back_Btn_on.png", "MainMenuUI/Medal/Back_Btn_on.png");
	MedalMenuBack->SetDelegate(this);
	MedalMenuBack->SetTag(BACK_BTN);
	m_pMedalUIRoot->AddChild(MedalMenuBack);


}

void cFirstScene::SoundBarControl()
{
	if (m_bIsLButtonDown)
	{
		if (PtInRect(&m_rSliderRect, m_ptMouse))
		{
			RECT rc;
			GetClientRect(g_hWnd, &rc);
			m_pSoundSlideBar->SetPosition(m_ptMouse.x, m_pSoundSlideBar->GetPosition().y);

		}
	}


	g_pSoundManager->SetSoundVol((m_pSoundSlideBar->GetPosition().x - 827) / 311); // 0 ~ 1.f  »ç¿îµå º¼·ý



}

void cFirstScene::LoadHoner(char * szFilename)
{

	FILE* fp = NULL;
	fopen_s(&fp, szFilename, "r");
	int lvtemp;

	while (true)
	{
		if (feof(fp))break;

		char szTemp[1024] = { '\0', };
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == 'S')
		{

			sscanf_s(szTemp, "%*c %*c %*c %*c %*c %*c %*c %*c %*c %*c %d", &lvtemp);
			if (lvtemp >= 3) m_honer[0] = true;
			else m_honer[0] = false;
		}
		if (szTemp[0] == 'W')
		{

			sscanf_s(szTemp, "%*c %*c %*c %*c %*c %*c %*c %d", &lvtemp);
			if (lvtemp >= 3) m_honer[1] = true;
			else m_honer[1] = false;
		}
		if (szTemp[0] == '9')
		{

			sscanf_s(szTemp, "%*c %*c %*c %*c %*c %*c %d", &lvtemp);
			if (lvtemp >= 3) m_honer[2] = true;
			else m_honer[2] = false;
		}
	}


	fclose(fp);

}

void cFirstScene::LoadAccuracyRate(char * szFilename)
{
	FILE* fp = NULL;
	fopen_s(&fp, szFilename, "r");

	while (true)
	{
		if (feof(fp))break;

		char szTemp[1024] = { '\0', };
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == 'G')
			m_honer[3] = true;
		if (szTemp[0] == 'B')
			m_honer[3] = false;

	}
	fclose(fp);
}

void cFirstScene::LoadHeathRate(char * szFileName)
{
	FILE* fp = NULL;
	fopen_s(&fp, szFileName, "r");


	while (true)
	{
		if (feof(fp))break;

		char szTemp[1024] = { '\0', };
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == 'G')
			m_honer[4] = true;
		if (szTemp[0] == 'B')
			m_honer[4] = false;

	}
	fclose(fp);
}

void cFirstScene::LoadPlayTime(char * szFilename)
{
	FILE* fp = NULL;
	fopen_s(&fp, szFilename, "r");


	while (true)
	{
		if (feof(fp))break;

		char szTemp[1024] = { '\0', };
		fgets(szTemp, 1024, fp);

		char timeTmp[1024];

		sscanf(szTemp, "%s", timeTmp);

		float time = atof(timeTmp);
		if (time > 3600.f)
		{
			m_honer[5] = true;
		}
		else m_honer[5] = false;

	}
	fclose(fp);
}
