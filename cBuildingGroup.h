#pragma once
class cBuilding;

class cBuildingGroup : public cObject
{
	std::vector<cBuilding*>		m_pvBuildings;

	SYNTHESIZE(D3DXVECTOR3, m_vCenter, Center);			// group�� �߽� ��ġ
	SYNTHESIZE(float,m_fScope, Scope);					// ai�� ���� ��ġ�� ���ü� �ִ� ����
	SYNTHESIZE(float, m_fBuildingScope, BuildingScope);	// building�� �߽���ġ�� ������ �ִ� ���� -> �� �ȿ� ����ִ� building�� ���Ϳ� �߰�����.

public:
	cBuildingGroup();
	~cBuildingGroup();

	void AddBuilding(cBuilding* building);
	std::vector<cBuilding*> GetBuilding();
};

