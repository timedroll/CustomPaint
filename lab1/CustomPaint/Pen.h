#pragma once
#include "ITool.h"
class Pen :
	public ITool
{
public:
	Pen(HWND hWnd);
	~Pen();
	void OnMouseMove(HDC hdcmem, LPARAM lParam);
	void OnLButtonDown(HDC hdcmem, LPARAM lParam);
	void OnLButtonUp(HDC hdcmem, LPARAM lParam);
private:
	HWND hWnd;
	bool IsDrawing = false;
	int xInit = 0, yInit = 0;
};


