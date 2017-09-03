#pragma once
class cScene/* : public cObject*/
{
public:
	cScene();
	virtual ~cScene();

	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

