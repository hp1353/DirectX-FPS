#pragma once
class cStaticMesh;

class cPlane
{
private:
	cStaticMesh*      m_pPlane;

	float             planeZ;
	D3DXVECTOR3       m_vPosition;
	D3DXMATRIXA16     m_matWorld;

public:
	cPlane();
	~cPlane();

	void Setup(char* szFolder, char* szFilename);
	void Update();
	void Render();
	void SetPosition(D3DXVECTOR3 pos);

};

