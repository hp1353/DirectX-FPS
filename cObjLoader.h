#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
private:
	std::map<std::string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(OUT std::vector<cGroup*>& vecGroup,
		IN char* szFolder, IN char* szFile);
	LPD3DXMESH LoadMesh(OUT std::vector<cMtlTex*>& vecMtlTex,
		IN char* szFolder, IN char* szFile);
	void LoadMtlLib(char* szFolder, char* szFile);
	void LoadSurface(OUT std::vector<D3DXVECTOR3>& vecSurface,
		IN char* szFolder, IN char* szFile,
		IN D3DXMATRIXA16* pmat = NULL);
};

