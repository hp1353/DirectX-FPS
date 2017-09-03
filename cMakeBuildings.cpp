#include "stdafx.h"
#include "cMakeBuildings.h"
#include "cBuilding.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cStaticMesh.h"
#include "cHeightMap.h"


cMakeBuildings::cMakeBuildings()
	: m_pBuilding(NULL)
	, m_pHeightMap(NULL)
	, m_nBuildingNum(0)
{
}


cMakeBuildings::~cMakeBuildings()
{
	SAFE_DELETE(m_pHeightMap);

	for each (auto p in m_vpAllBuildings)
		SAFE_RELEASE(p);

	for each (auto p in m_vpSettingBuildings)
		SAFE_RELEASE(p);

	m_pBuilding = NULL;
	m_pName.clear();
	m_vpAllBuildings.clear();
	m_vpSettingBuildings.clear();
}

void cMakeBuildings::Setup()
{
	//==buildings
	m_pName.push_back("barbwire1");
	m_pName.push_back("barrel1");
	m_pName.push_back("barrel2");
	m_pName.push_back("boat");
	m_pName.push_back("conveyer");
	m_pName.push_back("fueltank");
	m_pName.push_back("industrialtank");
	m_pName.push_back("refuse");
	m_pName.push_back("train");
	m_pName.push_back("chair1");
	m_pName.push_back("crate1");
	m_pName.push_back("crate2");
	m_pName.push_back("crate3");
	m_pName.push_back("cross1");
	m_pName.push_back("fountain1");
	m_pName.push_back("gravestone1");
	m_pName.push_back("pillar1");
	m_pName.push_back("pillar2");
	m_pName.push_back("pillar3");
	m_pName.push_back("pillar4");
	m_pName.push_back("pillar5");
	m_pName.push_back("pole1");
	m_pName.push_back("sandbag1");
	m_pName.push_back("square1");
	m_pName.push_back("statue1"); 
	m_pName.push_back("statue2");
	m_pName.push_back("statue4");
	m_pName.push_back("well1");
	m_pName.push_back("wall1");
	m_pName.push_back("wall2");
	m_pName.push_back("wall3");
	m_pName.push_back("wall4");
	m_pName.push_back("fence1");
	m_pName.push_back("cart1");
	m_pName.push_back("trashcan1");
	m_pName.push_back("car1"); 
	m_pName.push_back("car2");
	m_pName.push_back("car3");
	m_pName.push_back("car4");
	m_pName.push_back("car5");
	m_pName.push_back("car6");
	m_pName.push_back("car7");
	m_pName.push_back("brokenbuilding");
	m_pName.push_back("church");
	m_pName.push_back("cubestone");
	m_pName.push_back("woodenspike");
	m_pName.push_back("building1"); 
	m_pName.push_back("building2");
	m_pName.push_back("building4");
	m_pName.push_back("building5");
	m_pName.push_back("building6");
	m_pName.push_back("building7");
	m_pName.push_back("building8");
	m_pName.push_back("building9");
	m_pName.push_back("building10");
	m_pName.push_back("building11");
	m_pName.push_back("building12");
	m_pName.push_back("building13");
	m_pName.push_back("building14");
	m_pName.push_back("building15");
	m_pName.push_back("building17");
	m_pName.push_back("building18");
	m_pName.push_back("building19");
	m_pName.push_back("building22");
	m_pName.push_back("building23");
	m_pName.push_back("building24");
	m_pName.push_back("building25");
	m_pName.push_back("tree1");
	m_pName.push_back("tree2");
	m_pName.push_back("tree3");
	m_pName.push_back("tile");
	m_pName.push_back("log");
	m_pName.push_back("signstreet");
	m_pName.push_back("streetlight");



	for (size_t i = 0; i < m_pName.size(); i++)
	{
		cBuilding* pBuilding = new cBuilding("Map/building/", m_pName[i]);
		pBuilding->SetOBB("Map/building/", m_pName[i]);
		pBuilding->SetScale(0.007);
		m_vpAllBuildings.push_back(pBuilding);
	}

	m_pBuilding = m_vpAllBuildings[m_nBuildingNum];

	m_pHeightMap = new cHeightMap;
	m_pHeightMap->SetupText("Map/", "heightMap.txt", "Ground_CMGround_CM.tga");

}

void cMakeBuildings::Update()
{
	BuildingController();

	if (m_pBuilding)
		m_pBuilding->Update();

	for (size_t i = 0; i < m_vpSettingBuildings.size(); i++)
	{
		m_vpSettingBuildings[i]->Update();
	}

}

void cMakeBuildings::Render()
{

	if (m_pHeightMap)
		m_pHeightMap->Render();

	for (size_t i = 0; i < m_vpSettingBuildings.size(); i++)
	{
		m_vpSettingBuildings[i]->Render();
	}

	if (m_pBuilding)
		m_pBuilding->Render();

	char szTemp[1024];
	sprintf(szTemp, "building : %s\nCAMERA : W(up) S(down) A(left) D(right) \nBUILDING : Y(prev) U(next)\nROTATION : H(-) J (+) \nSCALE : N(-) M(+)\nPOSITION : UP(z+) DOWN(z-) LEFT(x-) RIGHT(x+) 1(y-) 2(y+) 3(y=0)\nSETTING : SPACE\nSAVE : ENTER", m_pName[m_nBuildingNum]);

	LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_DEFAULT);
	RECT rc;
	SetRect(&rc, 0, 100, 600, 300);
	pFont->DrawText(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 0));

}

void cMakeBuildings::BuildingController()
{
	D3DXVECTOR3 pos = m_pBuilding->GetPosition();

	if (g_pKeyManager->IsOnceKeyDown('Y'))
	{
		m_nBuildingNum = (--m_nBuildingNum + m_vpAllBuildings.size()) % m_pName.size();
		m_pBuilding = m_vpAllBuildings[m_nBuildingNum];
		m_pBuilding->Init();
		m_pBuilding->SetPosition(pos);
	}
	else if (g_pKeyManager->IsOnceKeyDown('U'))
	{
		m_nBuildingNum = (++m_nBuildingNum) % m_pName.size();
		m_pBuilding = m_vpAllBuildings[m_nBuildingNum];
		m_pBuilding->Init();
		m_pBuilding->SetPosition(pos);
	}
	else if (g_pKeyManager->IsStayKeyDown('H'))
	{
		m_pBuilding->SetAngle(m_pBuilding->GetAngle() - 0.03f);
	}
	else if (g_pKeyManager->IsStayKeyDown('J'))
	{
		m_pBuilding->SetAngle(m_pBuilding->GetAngle() + 0.03f);
	}
	else if (g_pKeyManager->IsOnceKeyDown('N'))
	{
		if (m_pBuilding->GetScale() > 0.002)
			m_pBuilding->SetScale(m_pBuilding->GetScale() - 0.001f);
	}
	else if (g_pKeyManager->IsOnceKeyDown('M'))
	{
		if (m_pBuilding->GetScale() < 0.02)
			m_pBuilding->SetScale(m_pBuilding->GetScale() + 0.001f);
	}
	
	if (g_pKeyManager->IsStayKeyDown(VK_LEFT))
		pos.x -= 0.1f;
	else if (g_pKeyManager->IsStayKeyDown(VK_RIGHT))
		pos.x += 0.1f;

	if (g_pKeyManager->IsStayKeyDown(VK_UP))
		pos.z += 0.1f;
	else if (g_pKeyManager->IsStayKeyDown(VK_DOWN))
		pos.z -= 0.1f;

	if (g_pKeyManager->IsStayKeyDown('1'))
		pos.y -= 0.1f;
	else if (g_pKeyManager->IsStayKeyDown('2'))
		pos.y += 0.1f;
	else if (g_pKeyManager->IsStayKeyDown('3'))
		pos.y = 0;

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
		AddBuilding();
	if (g_pKeyManager->IsOnceKeyDown(VK_RETURN))
		SaveMapFile();


	m_pBuilding->SetPosition(pos);
}

void cMakeBuildings::AddBuilding()
{
	cBuilding* pBuilding = new cBuilding("Map/Building/", (char*)m_vpAllBuildings[m_nBuildingNum]->GetFileName().c_str());
	pBuilding->SetScale(m_pBuilding->GetScale());
	pBuilding->SetAngle(m_pBuilding->GetAngle());
	pBuilding->SetPosition(m_pBuilding->GetPosition());
	m_vpSettingBuildings.push_back(pBuilding);

}

void cMakeBuildings::SaveMapFile()
{
	HANDLE file;

	char str[MAPMAXSIZE] = { 0 };
	DWORD write;

	file = CreateFile("mapFile.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	for (size_t i = 0; i < m_vpSettingBuildings.size(); i++)
	{
		strcat_s(str, "{\nDirectoryName : ");
		strcat_s(str, m_vpSettingBuildings[i]->GetFolderName().c_str());
		strcat_s(str, "\nFileName : ");
		strcat_s(str, m_vpSettingBuildings[i]->GetFileName().c_str());
		strcat_s(str, "\nS : ");
		strcat_s(str, std::to_string(m_vpSettingBuildings[i]->GetScale()).c_str());
		strcat_s(str, "\nA : ");
		strcat_s(str, std::to_string(m_vpSettingBuildings[i]->GetAngle()).c_str());
		strcat_s(str, "\nP : ");
		strcat_s(str, std::to_string(m_vpSettingBuildings[i]->GetPosition().x).c_str());
		strcat_s(str, ", ");
		strcat_s(str, std::to_string(m_vpSettingBuildings[i]->GetPosition().y).c_str());
		strcat_s(str, ", ");
		strcat_s(str, std::to_string(m_vpSettingBuildings[i]->GetPosition().z).c_str());
		strcat_s(str, "\n}\n");

	}
	
	WriteFile(file, str, MAPMAXSIZE, &write, NULL);
	CloseHandle(file);
}
