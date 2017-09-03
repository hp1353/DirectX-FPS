#pragma once
#include "cScene.h"

class cUIObject;

class cLoadingScene : public cScene
{
	cUIObject*			m_pMainUIRoot;
	LPD3DXSPRITE		m_pSprite;

public:
	cLoadingScene();
	~cLoadingScene();

	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
};

