#include "stdafx.h"
#include "cShaderLoader.h"


cShaderLoader::cShaderLoader()
{
}


cShaderLoader::~cShaderLoader()
{
}

LPD3DXEFFECT cShaderLoader::LoadEffect(char * szFilename)
{
	LPD3DXEFFECT pEffect;

	//에러 내용을 받아올 에러 버퍼
	LPD3DXBUFFER pError = NULL;

	//셰이더를 로딩과 동시에 컴파일 한다.
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				//디바이스를 넘긴다.
		szFilename,	//로딩될 셰이더 코드 경로 이름
		NULL,				//매크로 설정 안씀 NULL
		NULL,				//포함설정 안씀 NULL, 
		0,					//플레그 0
		NULL,				//로딩 메모리 풀 그냥  NULL
		&pEffect,			//로딩된 Effect 가 저장될 LPD3DXEFFECT 더블포인터
		&pError				//Error 버퍼 ( 만약 LPD3DXBUFFER 의 주소가 대입되면 컴파일과정중 에러가 나면 여기 에러메시지가 들어간다 제대로 컴파일되면 NULL 이 된다 )
	);

	//셰이더 코드 컴파일 로딩중 문제가 있다...
	if (pError != NULL && pEffect == NULL)
	{
		char* temp = (char*)pError->GetBufferPointer();
		//사용된 버퍼는 해재하자.
		SAFE_RELEASE(pError);

		return NULL;
	}


	return pEffect;
	
}
