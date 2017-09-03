#pragma once
class iMap;
class cHeightMap;
class cTextMap;
class cOBB;
class cCharacter;

class cController : public cObject
{
protected:
	float			m_fMoveSpeed;
	cOBB*			m_pOBB;

	cHeightMap*		m_pHeightMap;
	cTextMap*		m_pTextMap;

public:
	cController();
	virtual ~cController();

	virtual void Setup(float moveSpeed = 0.1f, cCharacter* pCharacter = NULL) PURE;
	virtual void Update(OUT D3DXVECTOR3& camAngle, OUT D3DXVECTOR3& vDirection, OUT D3DXVECTOR3& vPosition) PURE;

	virtual void SetOBB(cOBB* pObb);
	virtual void SetHeightMap(cHeightMap* pHeightMap);
	virtual void SetTextMap(cTextMap* pTextMap);
	
};

