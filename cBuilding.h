#pragma once
class cStaticMesh;
class cOBB;
class cGroup;

class cBuilding : public cObject
{
	cStaticMesh*		m_pBuilding;
	std::string			m_sFolderName;
	std::string			m_sFileName;
	cOBB*				m_pOBB;	/// >> : OBB

	std::vector<cGroup*> m_vecGroup;

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(float, m_fAngle, Angle);
	//SYNTHESIZE(float, m_fScale, Scale);
	float				m_fScale;

public:
	cBuilding(char* szFolder, char* szFilename);
	~cBuilding();

	void Setup(char* szFolder, char* szFilename);
	void Update();
	void Render();

	void Init();
	void SetOBB(char* szFolder, char* szFilename);
	float GetScale() { return m_fScale; }
	void SetScale(float scale);

	std::string GetFolderName() { return m_sFolderName; }
	std::string GetFileName() { return m_sFileName; }
	cOBB* GetOBB() { return m_pOBB; }
};

