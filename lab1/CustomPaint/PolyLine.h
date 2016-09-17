#pragma once
#include "ITool.h"
class PolyLine :
	public ITool
{
public:
	PolyLine(HWND hWnd);
	~PolyLine();
	void OnMouseMove(HDC hdcmem, LPARAM lParam);
	void OnLButtonDown(HDC hdcmem, LPARAM lParam);
	void OnLButtonUp(HDC hdcmem, LPARAM lParam);
	void OnRButtonDown(HDC hdcmem, LPARAM lParam);
private:
	HWND hWnd;
	bool IsDrawing = false;
	int xInit = 0, yInit = 0;
	int xPrev = 0, yPrev = 0;
};


