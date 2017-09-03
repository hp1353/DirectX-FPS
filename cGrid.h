#pragma once

class cPyramid;

class cGrid
{
private:
	
	int							m_nNumLine;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	std::vector<cPyramid*>		m_vecPyramid;

public:
	cGrid(void);
	~cGrid(void);

	void Setup(int nNumHalfTile = 15, float fInterval = 1.0f);
	void Render();
};

