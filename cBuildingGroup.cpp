#include "stdafx.h"
#include "cBuildingGroup.h"
#include "cBuilding.h"


cBuildingGroup::cBuildingGroup()
	: m_vCenter(0, 0, 0)
	, m_fScope(20.f)
	, m_fBuildingScope(25.f)
{
}


cBuildingGroup::~cBuildingGroup()
{
	m_pvBuildings.clear();
}

void cBuildingGroup::AddBuilding(cBuilding * building)
{
	D3DXVECTOR3 vBuildingPos = building->GetPosition();
	
	if ((vBuildingPos.x >= m_vCenter.x - m_fBuildingScope)
		&& (vBuildingPos.x <= m_vCenter.x + m_fBuildingScope)
		&& (vBuildingPos.z <= m_vCenter.z + m_fBuildingScope)
		&& (vBuildingPos.z >= m_vCenter.z - m_fBuildingScope))
	{
		m_pvBuildings.push_back(building);
	}

}

std::vector<cBuilding*> cBuildingGroup::GetBuilding()
{
	return m_pvBuildings;
}
