#include "stdafx.h"
#include "cRenderTexture.h"


cRenderTexture::cRenderTexture()
	: pRenderTexture(NULL)
	, pRenderSurface(NULL)
	, useDepthAndStencil(FALSE)
{
}


cRenderTexture::~cRenderTexture()
{
	Release();
}



void cRenderTexture::Init(int width, int height, D3DFORMAT format)
{

	this->width = width;
	this->height = height;

	//
	// RenderTarget 용 빈 Texture 만들기
	//
	g_pD3DDevice->CreateTexture(
		width,					//Texture 가로 해상도 
		height,					//Texture 세로 해상도
		1,							//밉맵체인 레벨
		D3DUSAGE_RENDERTARGET,		//RenderTarget 용 Texture이다
		format,						//TEXTURE 포맷 RenderTexture 용은D3DFMT_A8R8G8B8 로하자
		D3DPOOL_DEFAULT,			//RenderTarget 용 Texture 는 Pool 을 Default
		&pRenderTexture,			//생성된 Texture 받아올 포인터
		NULL						//신경쓰지말고 NULL
	);

	//
	// Render 할 Surface ( 이게 DepthBuffer 와 StencilBuffer 를 대신한다 )
	//
	g_pD3DDevice->CreateDepthStencilSurface(
		width,					//Texture 가로 해상도 
		height,					//Texture 세로 해상도
		D3DFMT_D24S8,			//Deapth 는 24 비트 Stencil 은 8 비트	
		D3DMULTISAMPLE_NONE,		//멀티 샘플링 안티알리아싱은 존재하지 않는다, 
		0,							//멀티 샘플링 퀄리티는 0
		TRUE,						//버퍼 교체시 이전 퍼버내용을 유지하지 않니? ( TRUE 로 쓰면 버퍼교체 될때 이전에 써진 버퍼내용을 기억하지 않는다 )
		&pRenderSurface,			//얻어올 포인터...
		NULL						//신경쓰지말고 NULL
	);

}
void cRenderTexture::Release()
{
	SAFE_RELEASE(pRenderTexture);
	SAFE_RELEASE(pRenderSurface);
}

void cRenderTexture::BeginRender(
	DWORD stage,
	DWORD clearColor,
	DWORD clearFlag,
	BOOL useDepthAndStencil)
{
	this->useDepthAndStencil = useDepthAndStencil;
	this->stageNum = stage;

	//
	// 화면에 랜더링 되는 대신 Texture에 랜더링한다.
	//

	//현 디바이스의 Target 버퍼의표면과 DepthStencil 버퍼의 표면정보를 기억

	g_pD3DDevice->GetRenderTarget(this->stageNum, &pDeviceTargetSurface);				//현재 Device 의 0 번 랜더 TargetSurface 를 기억
	g_pD3DDevice->GetDepthStencilSurface(&pDeviceDepthAndStencilSurface);	//현재 Device 의 스텐실 버퍼와 뎁스퍼퍼 표면기억


																	//
																	// TargetBuffer 를 RenderTexture 의 Surface 로 셋팅
																	//

																	//RenderTexture 의 Surface 를 얻는다.
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(this->pRenderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//성공적으로 RenderTexture 의 Surface 를 얻었다면....

		//Texture 표면을 Device 의 Target 버퍼로 셋팅한다.
		g_pD3DDevice->SetRenderTarget(this->stageNum, texSurface);

		//셋팅된 Surface 정보는 바로 날려주는 예의를 갖추자...
		SAFE_RELEASE(texSurface);
	}


	if (this->useDepthAndStencil)
	{
		//
		// Depth 버퍼와 Stencil 버퍼의 Surface 을 pRenderSurface 로 셋팅
		//
		g_pD3DDevice->SetDepthStencilSurface(pRenderSurface);
	}


	//디바이스 버퍼 클리어 ( 사실 위에서 Setting 된 Textuer 랑 Surface 가 클리어 된다  )
	g_pD3DDevice->Clear(0, NULL,
		clearFlag,
		clearColor,
		1.0f,
		0);

}

void cRenderTexture::EndRender()
{

	//
	// 뒤처리 ( 원래대로 돌려놓는다 )
	//
	//Render Texture 에 그릴 작업이 끝났으면 다시 원상복귀하는 센스....
	g_pD3DDevice->SetRenderTarget(this->stageNum, pDeviceTargetSurface);

	//셋팅된 Surface 정보는 바로 날려주는 예의를 갖추자...
	SAFE_RELEASE(pDeviceTargetSurface);

	if (this->useDepthAndStencil)
	{
		g_pD3DDevice->SetDepthStencilSurface(pDeviceDepthAndStencilSurface);
		SAFE_RELEASE(pDeviceDepthAndStencilSurface);
	}
}