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

	//���� ������ �޾ƿ� ���� ����
	LPD3DXBUFFER pError = NULL;

	//���̴��� �ε��� ���ÿ� ������ �Ѵ�.
	D3DXCreateEffectFromFile(
		g_pD3DDevice,				//����̽��� �ѱ��.
		szFilename,	//�ε��� ���̴� �ڵ� ��� �̸�
		NULL,				//��ũ�� ���� �Ⱦ� NULL
		NULL,				//���Լ��� �Ⱦ� NULL, 
		0,					//�÷��� 0
		NULL,				//�ε� �޸� Ǯ �׳�  NULL
		&pEffect,			//�ε��� Effect �� ����� LPD3DXEFFECT ����������
		&pError				//Error ���� ( ���� LPD3DXBUFFER �� �ּҰ� ���ԵǸ� �����ϰ����� ������ ���� ���� �����޽����� ���� ����� �����ϵǸ� NULL �� �ȴ� )
	);

	//���̴� �ڵ� ������ �ε��� ������ �ִ�...
	if (pError != NULL && pEffect == NULL)
	{
		char* temp = (char*)pError->GetBufferPointer();
		//���� ���۴� ��������.
		SAFE_RELEASE(pError);

		return NULL;
	}


	return pEffect;
	
}
