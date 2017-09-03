#pragma once

#define g_pSceneManager cSceneManager::GetInstance()

class cScene;

class cSceneManager
{
private:
	friend DWORD CALLBACK LoadingThread(LPVOID lpParam);
	typedef std::map<std::string, cScene*>		MAP_SCENE;

	cScene*			m_pNowScene;			//���� ���� �ִ¾�
	cScene*			m_pReleaseScene;		//���� ��
	MAP_SCENE		m_Scenes;				//���� ������ �ִ¾�
	MAP_SCENE		m_LoadingScenes;		//�ε�����

public:
	float			fLoadingProgress;			//�ε� ���൵ 0 ~ 1
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

