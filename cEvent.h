#pragma once

class cUIImageView;
class cUIObject;

class cEvent : public cObject
{
	LPD3DXSPRITE		m_pSprite;

	LPDIRECT3DTEXTURE9	m_aTexture[2];
	int					m_nTexturNum;
	int					m_nWidth[2];
	int					m_nHeight[2];

	int                 m_nCount;
	int					m_nFadeInSpeed;
	int					m_nFadeOutSpeed;
	float				m_fAlpha;
	float				m_fDeltaAlpha;

	SYNTHESIZE(float, m_fAngle, Angle);

	SYNTHESIZE(bool, m_isPlay, IsPlay);

public:
	cEvent();
	~cEvent();

	void Setup(char * FirstszFullPath, char * SecondszFullPath, int fadeInSpeed = 120, int fadeOutSpeed = 120);
	void Update();
	void Render();

};

