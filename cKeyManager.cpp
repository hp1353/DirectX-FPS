#include "stdafx.h"
#include "cKeyManager.h"


cKeyManager::cKeyManager()
{
	for (int i = 0; i < MAX_KEY_NUM; i++) {
		m_bDownCheck[i] = false;
		m_bUpCheck[i] = false;
	}
}


cKeyManager::~cKeyManager()
{
}

bool cKeyManager::IsStayKeyDown(int nVKey)
{
	return (GetAsyncKeyState(nVKey) & 0x8000);
}

bool cKeyManager::IsOnceKeyDown(int nVKey)
{
	if (GetAsyncKeyState(nVKey) & 0x8000) {

		if (m_bDownCheck[nVKey] == false) {
			m_bDownCheck[nVKey] = true;
			return true;
		}
	}

	else {
		m_bDownCheck[nVKey] = false;
	}

	return false;
}

bool cKeyManager::IsOnceKeyUp(int nVKey)
{
	if (GetAsyncKeyState(nVKey) & 0x8000) {
		if (m_bUpCheck[nVKey] == false) {
			m_bUpCheck[nVKey] = true;
		}
	}

	else {
		if (m_bUpCheck[nVKey]) {
			m_bUpCheck[nVKey] = false;
			return true;
		}
	}

	return false;
}
