#include "stdafx.h"
#include "cSceneManager.h"
#include "cScene.h"


cSceneManager::cSceneManager()
	: m_pNowScene(NULL)
	, m_pReleaseScene(NULL)
{
}


cSceneManager::~cSceneManager()
{
}

DWORD CALLBACK LoadingThread(LPVOID lpParam)
{
	cScene* pScene = (cScene*)lpParam;

	pScene->Setup();

	g_pSceneManager->fLoadingProgress = 1.0f;

	g_pSceneManager->m_pNowScene = pScene;

	return 0;
}

void cSceneManager::Setup()
{
}

void cSceneManager::Destroy()
{
	MAP_SCENE::iterator iter;

	for (iter = m_LoadingScenes.begin(); iter != m_LoadingScenes.end(); iter++)
	{
		if (iter->second == m_pNowScene)
		{
			m_pNowScene = NULL;
		}

		iter->second->Destroy();
		SAFE_DELETE(iter->second);
	}

	if (m_pNowScene != NULL)
		m_pNowScene->Destroy();

	for (iter = m_Scenes.begin(); iter != m_Scenes.end(); iter++)
	{
		SAFE_DELETE(iter->second);
	}


}

void cSceneManager::Update(float timeDelta)
{
	if (m_pReleaseScene != NULL)
	{
		//SAFE_RELEASE(m_pReleaseScene);
		m_pReleaseScene->Destroy();
		m_pReleaseScene = NULL;
	}

	if (m_pNowScene != NULL)
		m_pNowScene->Update();
}

void cSceneManager::Render()
{
	if (m_pNowScene != NULL) 
		m_pNowScene->Render();
}

void cSceneManager::AddScene(std::string sceneName, cScene * pScene)
{
	MAP_SCENE::iterator  iter = m_Scenes.find(sceneName);

	if (iter == m_Scenes.end()) {
		m_Scenes.insert(std::make_pair(sceneName, pScene));
	}
}

void cSceneManager::AddLoadingScene(std::string sceneName, cScene * pScene)
{
	MAP_SCENE::iterator  iter = m_LoadingScenes.find(sceneName);

	if (iter == m_LoadingScenes.end()) {
		pScene->Setup();
		m_LoadingScenes.insert(std::make_pair(sceneName, pScene));
	}
}

void cSceneManager::ChangeScene(std::string sceneName)
{
	MAP_SCENE::iterator iter = m_Scenes.find(sceneName);

	if (iter == m_Scenes.end())
		return;

	m_pReleaseScene = m_pNowScene;
	
	m_pNowScene = iter->second;
	m_pNowScene->Setup();
}

void cSceneManager::ChangeSceneWithLoading(std::string sceneName, std::string loadingSceneName)
{
	MAP_SCENE::iterator
		iter = m_LoadingScenes.find(loadingSceneName);

	if (iter == m_LoadingScenes.end()) {
		return;
	}

	cScene* pLoadingScene = iter->second;

	iter = m_Scenes.find(sceneName);

	if (iter == m_Scenes.end()) {
		return;
	}

	cScene* pNextScene = iter->second;

	fLoadingProgress = 0.0f;
	
	DWORD dwThreadID;
	HANDLE hThread;

	hThread = CreateThread(
		NULL,					//쓰레드 속성 일단 NULL				
		0,						//쓰레드의 스텍 사이즈 (0 으로하면 MainThread 와 같은 1MB 가 된다 )
		LoadingThread,			//새롭게생성되는 쓰레드의 메인함수 CALLBACK 함수포인터
		pNextScene,				//새롭게생성되는 쓰레드의 메인함수의 매개변수 포인터 ( LoadingThread 의 매개변수로 들어간다 )
		0,						//쓰레드 플레그 ( 0 으로 주면 바로 LoadingThread 가실행 )
		&dwThreadID				//만들어진 쓰레드 ID 
	);

	CloseHandle(hThread);

	m_pReleaseScene = m_pNowScene;
	m_pNowScene = pLoadingScene;
}

void cSceneManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pNowScene)
		m_pNowScene->WndProc(hWnd, message, wParam, lParam);
}

bool cSceneManager::CurrentScene(std::string sceneName)
{
	MAP_SCENE::iterator find = m_Scenes.find(sceneName);
	return (find->second == m_pNowScene);
}
