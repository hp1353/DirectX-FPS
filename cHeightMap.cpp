#include "StdAfx.h"
#include "cHeightMap.h"


cHeightMap::cHeightMap(void)
	: m_pTexture(NULL)
	, m_pMesh(NULL)
	, m_nMapSize(0)
{
}

cHeightMap::~cHeightMap(void)
{
	SAFE_RELEASE(m_pMesh);
}

// >> :  
void cHeightMap::Setup(char* szFolder,
	char* szRaw,
	char* szTex,
	DWORD dwBytePerPixel /*= 1*/)
{
	std::string sFolder(szFolder);
	std::string sRaw = sFolder + std::string(szRaw);
	std::string sTex = sFolder + std::string(szTex);

	m_pTexture = g_pTextureManager->GetTexture(sTex);

	FILE* fp = NULL;
	fopen_s(&fp, sRaw.c_str(), "r");

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);

	int nNumVertex = nFileSize / dwBytePerPixel;

	int nRow = (int)(sqrt((float)nNumVertex) + 0.0001f);
	int nCol = nRow;
	int nTileN = nRow - 1; // : 256

	m_nMapSize = nRow;
	m_nTileN = nTileN;

	fseek(fp, 0, SEEK_SET);

	std::vector<ST_PNT_VERTEX>	vecVertex(nNumVertex);		/// : ST_PNT_VERTEX 정의 해놓은거 없으면 새로 정의 한다.. 물어볼것..
	m_vecVertex.resize(nNumVertex);


	std::vector<DWORD>			vecIndex;
	vecIndex.reserve(nTileN * nTileN * 2 * 3);
	for (int i = 0; i < nNumVertex; ++i)
	{
		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3(i % nCol, ((unsigned char)fgetc(fp)) / 10.0f, i / nCol);
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i % nCol) /*/ (float)nCol*/, (i / nCol)/* / (float)nCol*/);
		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
		if (dwBytePerPixel == 3)
		{
			fgetc(fp); fgetc(fp);
		}
	}
	fclose(fp);

	for (int x = 1; x < nTileN; ++x)
	{
		for (int z = 1; z < nTileN; ++z)
		{
			int left = (z + 0) * nCol + x - 1;
			int right = (z + 0) * nCol + x + 1;
			int up = (z + 1) * nCol + x + 0;
			int down = (z - 1) * nCol + x + 0;

			D3DXVECTOR3 leftToRight = m_vecVertex[right] - m_vecVertex[left];
			D3DXVECTOR3 downToUp = m_vecVertex[up] - m_vecVertex[down];
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &downToUp, &leftToRight);
			D3DXVec3Normalize(&normal, &normal);

			int nIndex = z * nCol + x;
			vecVertex[nIndex].n = normal;
		}
	}

	for (int x = 0; x < nTileN; ++x)
	{
		for (int z = 0; z < nTileN; ++z)
		{
			int _0 = (z + 0) * nCol + x + 0;
			int _1 = (z + 1) * nCol + x + 0;
			int _2 = (z + 0) * nCol + x + 1;
			int _3 = (z + 1) * nCol + x + 1;

			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}

	///>> : 
	D3DXCreateMeshFVF(
		vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	/// << : 
} // << : func setup()

void cHeightMap::SetupText(char * szFolder, char * szTxt, char * szTex, DWORD dwBytePerPixel)
{
	// 129 x 129 으로 고정된 txt파일 

	std::string sFolder(szFolder);
	std::string sTxt = /*sFolder + */std::string(szTxt);
	std::string sTex = sFolder + std::string(szTex);

	m_pTexture = g_pTextureManager->GetTexture(sTex);

	FILE* fp = NULL;
	fopen_s(&fp, sTxt.c_str(), "r");

	int nNumVertex = MAPSIZE * MAPSIZE;
	//int nRow = MAPSIZE;
	//int nCol = MAPSIZE;
	//int nTileN = MAPSIZE - 1;

	m_nMapSize = MAPSIZE;
	m_nTileN = MAPSIZE - 1;

	std::vector<ST_PNT_VERTEX>	vecVertex(nNumVertex);		/// : ST_PNT_VERTEX 정의 해놓은거 없으면 새로 정의 한다.. 물어볼것..
	m_vecVertex.resize(nNumVertex);

	std::vector<DWORD>			vecIndex;
	vecIndex.reserve(m_nTileN * m_nTileN * 2 * 3);
	for (int i = 0; i < nNumVertex; ++i)
	{
		int vertex;
		fscanf_s(fp, "%d", &vertex);

		ST_PNT_VERTEX v;
		v.p = D3DXVECTOR3(i % MAPSIZE - (MAPSIZE / 2), vertex / 10.0f, i / MAPSIZE - (MAPSIZE / 2));
		v.n = D3DXVECTOR3(0, 1, 0);
		v.t = D3DXVECTOR2((i % MAPSIZE) /*/ (float)MAPSIZE*/, (i / MAPSIZE)/* / (float)MAPSIZE*/);
		vecVertex[i] = v;
		m_vecVertex[i] = v.p;
		
	}
	fclose(fp);

	for (int x = 1; x < m_nTileN; ++x)
	{
		for (int z = 1; z < m_nTileN; ++z)
		{
			int left = (z + 0) * MAPSIZE + x - 1;
			int right = (z + 0) * MAPSIZE + x + 1;
			int up = (z + 1) * MAPSIZE + x + 0;
			int down = (z - 1) * MAPSIZE + x + 0;

			D3DXVECTOR3 leftToRight = m_vecVertex[right] - m_vecVertex[left];
			D3DXVECTOR3 downToUp = m_vecVertex[up] - m_vecVertex[down];
			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &downToUp, &leftToRight);
			D3DXVec3Normalize(&normal, &normal);

			int nIndex = z * MAPSIZE + x;
			vecVertex[nIndex].n = normal;
		}
	}

	for (int x = 0; x < m_nTileN; ++x)
	{
		for (int z = 0; z < m_nTileN; ++z)
		{
			int _0 = (z + 0) * MAPSIZE + x + 0;
			int _1 = (z + 1) * MAPSIZE + x + 0;
			int _2 = (z + 0) * MAPSIZE + x + 1;
			int _3 = (z + 1) * MAPSIZE + x + 1;

			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}

	///>> : 
	D3DXCreateMeshFVF(
		vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = NULL;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = NULL;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = NULL;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0.0f, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
}

void cHeightMap::Render()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	m_pMesh->DrawSubset(0);
}

bool cHeightMap::GetHeight(IN float x, OUT float& y, IN float z)
{
	// 소수점 이하 버리기
	int nX = x + m_nMapSize / 2;
	int nZ = z + m_nMapSize / 2;

	// : 맵 밖으로 나가지 말도록...
	if (nX < 0.f || nZ < 0.f || nX >= m_nTileN || nZ >= m_nTileN)
	{
		y = 0;
		return false;
	}

	// 소수 이하값만 구하기
	float fDeltaX = x - nX + m_nMapSize / 2;
	float fDeltaZ = z - nZ + m_nMapSize / 2;

	int _0 = (nZ + 0) * (m_nTileN + 1) + nX + 0;
	int _1 = (nZ + 1) * (m_nTileN + 1) + nX + 0;
	int _2 = (nZ + 0) * (m_nTileN + 1) + nX + 1;
	int _3 = (nZ + 1) * (m_nTileN + 1) + nX + 1;

	if (fDeltaX + fDeltaZ < 1.0f)	// 왼쪽 아래 삼각형
	{
		D3DXVECTOR3 v01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 v02 = m_vecVertex[_2] - m_vecVertex[_0];
		y = m_vecVertex[_0].y + (v01 * fDeltaZ + v02 * fDeltaX).y;
		return true;
	}
	else	//오른쪽 위 삼각형
	{
		fDeltaX = 1.0f - fDeltaX;
		fDeltaZ = 1.0f - fDeltaZ;
		D3DXVECTOR3 v31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 v32 = m_vecVertex[_2] - m_vecVertex[_3];
		y = m_vecVertex[_3].y + (v31 * fDeltaX + v32 * fDeltaZ).y;
		return true;
	}
	return true;
}

