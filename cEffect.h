#pragma once
class cEffect : public cObject
{
	char*						m_sFileName;
	std::vector<std::vector<ST_PCT_VERTEX>>	m_vecVertex;
	/*D3DXVECTOR3					m_vPosition;*/

	int							m_nFrameCount;
	int							m_nCurrentFrameX;
	int							m_nCurrentFrameY;
	int							m_nMaxFrameX;
	int							m_nMaxFrameY;

	SYNTHESIZE(bool	,m_isPlay, Play);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cEffect();
	~cEffect();

	void Setup(char* fileName, float width, float height, int frameX = 1, int frameY = 1);
	void Update();
	void Render();
	void Destroy();

	void SetRenderState();
	void SetBillboard(D3DXMATRIXA16 * matWorld);
};

