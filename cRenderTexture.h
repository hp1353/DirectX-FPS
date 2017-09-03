#pragma once
class cRenderTexture
{
private:
	LPDIRECT3DTEXTURE9	pRenderTexture;		//랜더링될 Texture
	LPDIRECT3DSURFACE9	pRenderSurface;		//랜더 Texture Deapth 버퍼와 Stencil 버퍼가 있는 Surface
	DWORD				stageNum;

	LPDIRECT3DSURFACE9		pDeviceTargetSurface;
	LPDIRECT3DSURFACE9		pDeviceDepthAndStencilSurface;

	BOOL				useDepthAndStencil;		//독자적인 Depth 채널과 Stencil 을 사용할꺼니?

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

