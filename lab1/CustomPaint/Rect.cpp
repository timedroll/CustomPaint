#include "stdafx.h"
#include "Rect.h"


Rect::Rect(HWND hWnd) : ITool(hWnd)
{
	this->hWnd = hWnd;
}


Rect::~Rect()
{
}

void Rect::OnLButtonDown(HDC hdcmem, LPARAM lParam)
{
	xInit = LOWORD(lParam);
	yInit = HIWORD(lParam);
	IsDrawing = true;
}

void Rect::OnLButtonUp(HDC hdcmem, LPARAM lParam)
{
	int xCur = LOWORD(lParam);
	int yCur = HIWORD(lParam);
	MoveToEx(hdcmem, xCur, yCur, NULL);
	IsDrawing = false;
	Rectangle(hdcmem, min(xInit, xCur), min(yInit, yCur), max(xInit, xCur), max(yInit, yCur));
	HDC hdc = GetDC(hWnd);
	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
}

void Rect::OnMouseMove(HDC hdcmem, LPARAM lParam)
{
	if (!IsDrawing)
		return;
	HDC hdc = GetDC(hWnd);

	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
	int xCur = LOWORD(lParam);
	int yCur = HIWORD(lParam);
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_PEN));
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_BRUSH));
	MoveToEx(hdc, xCur, yCur, NULL);
	Rectangle(hdc, min(xInit, xCur), min(yInit, yCur), max(xInit, xCur), max(yInit, yCur));
	ReleaseDC(hWnd, hdc);
}

