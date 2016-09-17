#pragma once
class ITool
{
public:
	ITool(HWND hWnd);
	~ITool();
	void virtual OnMouseMove(HDC hdcmem, LPARAM lParam) = 0;
	void virtual OnLButtonUp(HDC hdcmem, LPARAM lParam) = 0;
	void virtual OnLButtonDown(HDC hdcmem, LPARAM lParam) = 0;
	void virtual OnRButtonDown(HDC hdcmem, LPARAM lParam);
private:
	HWND hWnd;
	bool IsDrawing = false;
	int xInit = 0, yInit = 0;
};

