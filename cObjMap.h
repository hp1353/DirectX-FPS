#pragma once

class cObjMap : public iMap
{
private:
	std::vector<D3DXVECTOR3> m_vecSurface;

public:
	cObjMap(IN char* szFolder, IN char* szFile, IN D3DXMATRIXA16* pmat = NULL);
	~cObjMap(void);

	void Load(IN char* szFolder, IN char* szFile, IN D3DXMATRIXA16* pmat = NULL);

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
};

