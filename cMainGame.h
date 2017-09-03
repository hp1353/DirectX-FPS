#pragma once


class cScene;
#include "cUIButton.h"

class cMainGame
	: public iButtonDelegate
{
private:
	

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void SetLight();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;



};

