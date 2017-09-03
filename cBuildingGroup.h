#pragma once
class cBuilding;

class cBuildingGroup : public cObject
{
	std::vector<cBuilding*>		m_pvBuildings;

	SYNTHESIZE(D3DXVECTOR3, m_vCenter, Center);			// group의 중심 위치
	SYNTHESIZE(float,m_fScope, Scope);					// ai의 랜덤 위치가 나올수 있는 범위
	SYNTHESIZE(float, m_fBuildingScope, BuildingScope);	// building의 중심위치가 있을수 있는 범위 -> 이 안에 들어있는 building만 벡터에 추가해줌.

public:
	cBuildingGroup();
	~cBuildingGroup();

	void AddBuilding(cBuilding* building);
	std::vector<cBuilding*> GetBuilding();
};

