#include "stdafx.h"
#include "cMapToolScene.h"
#include "cGrid.h"
#include "cMapCamera.h"
#include "cBuilding.h"
#include "cUITextView.h"
#include "cStaticMesh.h"
#include "cMakeGround.h"
#include "cMakeBuildings.h"
#include "cRay.h"


cMapToolScene::cMapToolScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_mapMode(MAKE_BUILDINGS)
	, m_pSkyView(NULL)
	, m_pGroundMode(NULL)
	, m_pBuildingMode(NULL)
{
}


cMapToolScene::~cMapToolScene()
{
	
}

void cMapToolScene::Setup()
{
	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCamera = new cMapCamera;
	m_pCamera->Setup(NULL);

	m_pSkyView = new cStaticMesh("Map/Sky/", "sky.X");
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
	m_pSkyView->SetWorld(matS);

	m_pGroundMode = new cMakeGround;
	m_pGroundMode->Setup();

	m_pBuildingMode = new cMakeBuildings;
	m_pBuildingMode->Setup();


}

void cMapToolScene::Destroy()
{
	SAFE_DELETE(m_pBuildingMode);
	SAFE_DELETE(m_pGroundMode);
	SAFE_DELETE(m_pSkyView);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

}

void cMapToolScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	switch (m_mapMode)
	{
	case MAKE_GROUND:
		if (m_pGroundMode)
			m_pGroundMode->Update(m_pCamera->GetMousePosition());
		break;
	case MAKE_BUILDINGS:
		if (m_pBuildingMode)
			m_pBuildingMode->Update();
		break;
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_ESCAPE))
	{
		if (m_mapMode == MAKE_GROUND)
		{
			if (m_pGroundMode)
			{
				m_pGroundMode->SaveMapFile();
				m_mapMode = MAKE_BUILDINGS;
			}
		}
		else if (m_mapMode == MAKE_BUILDINGS)
		{
			if (m_pBuildingMode)
			{
				m_pBuildingMode->SaveMapFile();
				m_mapMode = MAKE_GROUND;
			}
		}
	}

}

void cMapToolScene::Render()
{

	//if (m_pSkyView)
	//	m_pSkyView->Render();

	switch (m_mapMode)
	{
	case MAKE_GROUND:
		if (m_pGroundMode)
			m_pGroundMode->Render();
		break;
	case MAKE_BUILDINGS:
		if (m_pBuildingMode)
			m_pBuildingMode->Render();
		break;
	}

	if (m_pGrid)
		m_pGrid->Render();

}

void cMapToolScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}
