#pragma once
class cRenderTexture
{
private:
	LPDIRECT3DTEXTURE9	pRenderTexture;		//�������� Texture
	LPDIRECT3DSURFACE9	pRenderSurface;		//���� Texture Deapth ���ۿ� Stencil ���۰� �ִ� Surface
	DWORD				stageNum;

	LPDIRECT3DSURFACE9		pDeviceTargetSurface;
	LPDIRECT3DSURFACE9		pDeviceDepthAndStencilSurface;

	BOOL				useDepthAndStencil;		//�������� Depth ä�ΰ� Stencil �� ����Ҳ���?

	DWORD				width;
	DWORD				height;

public:
	cRenderTexture();
	~cRenderTexture();

	void Init(int width, int height, D3DFORMAT format = D3DFMT_A8R8G8B8);
	void Release();

	void BeginRender(
		DWORD stage = 0,
		DWORD clearColor = 0xFF000000,
		DWORD clearFlag = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		BOOL useDepthAndStencil = FALSE);

	void EndRender();


	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return this->pRenderTexture;
	}

	DWORD GetWidth() {
		return this->width;
	}

	DWORD GetHeight() {
		return this->height;
	}
};

