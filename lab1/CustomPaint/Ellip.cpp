#include "stdafx.h"
#include "Ellip.h"


Ellip::Ellip(HWND hWnd) : ITool(hWnd)
{
	this->hWnd = hWnd;
}


Ellip::~Ellip()
{
}

void Ellip::OnLButtonDown(HDC hdcmem, LPARAM lParam)
{
	xInit = LOWORD(lParam);
	yInit = HIWORD(lParam);
	IsDrawing = true;
}

void Ellip::OnLButtonUp(HDC hdcmem, LPARAM lParam)
{
	int xCur = LOWORD(lParam);
	int yCur = HIWORD(lParam);
	MoveToEx(hdcmem, xCur, yCur, NULL);
	IsDrawing = false;
	Ellipse(hdcmem, min(xInit, xCur), min(yInit, yCur), max(xInit, xCur), max(yInit, yCur));
	HDC hdc = GetDC(hWnd);
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_PEN));
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_BRUSH));
	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
}

void Ellip::OnMouseMove(HDC hdcmem, LPARAM lParam)
{
	if (!IsDrawing)
		return;
	HDC hdc = GetDC(hWnd);
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_PEN));
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_BRUSH));
	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
	int xCur = LOWORD(lParam);
	int yCur = HIWORD(lParam);
	MoveToEx(hdc, xCur, yCur, NULL);
	Ellipse(hdc, min(xInit, xCur), min(yInit, yCur), max(xInit, xCur), max(yInit, yCur));
	ReleaseDC(hWnd, hdc);
}
