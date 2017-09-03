#pragma once

class cPyramid
{
private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	int							m_nNumTri;
	D3DXMATRIXA16				m_matWorld;
	D3DMATERIAL9				m_stMtl;

public:
	cPyramid(void);
	~cPyramid(void);

	void Setup(D3DXCOLOR c, D3DXMATRIXA16& mat);
	void Render();
};

