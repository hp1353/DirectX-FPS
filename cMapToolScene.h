#pragma once
#include "cScene.h"


class cBuilding;
class cGrid;
class cMapCamera;
class cStaticMesh;
class cMakeGround;
class cMakeBuildings;

enum MAP_MODE
{
	MAKE_GROUND,
	MAKE_BUILDINGS
};

class cMapToolScene : public cScene
{

	cMapCamera*					m_pCamera;
	cGrid*						m_pGrid;
	cStaticMesh*				m_pSkyView;

	MAP_MODE					m_mapMode;
	
	cMakeGround*				m_pGroundMode;
	cMakeBuildings*				m_pBuildingMode;

public:
	cMapToolScene();
	virtual ~cMapToolScene();

	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

