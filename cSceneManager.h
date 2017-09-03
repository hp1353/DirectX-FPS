#pragma once

#define g_pSceneManager cSceneManager::GetInstance()

class cScene;

class cSceneManager
{
private:
	friend DWORD CALLBACK LoadingThread(LPVOID lpParam);
	typedef std::map<std::string, cScene*>		MAP_SCENE;

	cScene*			m_pNowScene;			//현재 물려 있는씬
	cScene*			m_pReleaseScene;		//날릴 씬
	MAP_SCENE		m_Scenes;				//현재 가지고 있는씬
	MAP_SCENE		m_LoadingScenes;		//로딩씬들

public:
	float			fLoadingProgress;			//로딩 진행도 0 ~ 1
	SINGLETONE(cSceneManager);

public:
	
	void Setup();
	void Destroy();
	void Update(float timeDelta);
	void Render();
	

	void AddScene(std::string sceneName, cScene* pScene);
	void AddLoadingScene(std::string sceneName, cScene* pScene);

	void ChangeScene(std::string sceneName);
	void ChangeSceneWithLoading(
		std::string sceneName,
		std::string loadingSceneName);

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool CurrentScene(std::string sceneName);

};

