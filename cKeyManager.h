#pragma once

#define g_pKeyManager cKeyManager::GetInstance()
#define MAX_KEY_NUM		256

class cKeyManager
{
private:
	SINGLETONE(cKeyManager);

private:
	bool			m_bDownCheck[MAX_KEY_NUM];
	bool			m_bUpCheck[MAX_KEY_NUM];

public:
	bool IsStayKeyDown(int nVKey);
	bool IsOnceKeyDown(int nVKey);
	bool IsOnceKeyUp(int nVKey);
};

