#pragma once

#define MAPMAXSIZE 50000

class cBuilding;
class cUIObject;
class cStaticMesh;
class cHeightMap;

class cMakeBuildings
{
	cBuilding*					m_pBuilding;
	cHeightMap*					m_pHeightMap;

	std::vector<cBuilding*>		m_vpAllBuildings;
	std::vector<cBuilding*>		m_vpSettingBuildings;

	std::vector<char*>			m_pName;
	int							m_nBuildingNum;

public:
	cMakeBuildings();
	~cMakeBuildings();

	void Setup();
	void Update();
	void Render();

	void BuildingController();

	void AddBuilding();
	void SaveMapFile();
};

