#include "stdafx.h"
#include "Line.h"


Line::Line(HWND hWnd) : ITool(hWnd)
{
	this->hWnd = hWnd;
}


Line::~Line()
{
}

void Line::OnMouseMove(HDC hdcmem, LPARAM lParam)
{
	if (!IsDrawing)
		return;
	HDC hdc = GetDC(hWnd);
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_PEN));
	SelectObject(hdc, GetCurrentObject(hdcmem, OBJ_BRUSH));
	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
	MoveToEx(hdc, LOWORD(lParam), HIWORD(lParam), NULL);
	LineTo(hdc, xInit, yInit);
	ReleaseDC(hWnd, hdc);
}

void Line::OnLButtonDown(HDC hdcmem, LPARAM lParam)
{
	IsDrawing = true;
	xInit = LOWORD(lParam);
	yInit = HIWORD(lParam);
}

void Line::OnLButtonUp(HDC hdcmem, LPARAM lParam)
{
	MoveToEx(hdcmem, LOWORD(lParam), HIWORD(lParam), NULL);
	IsDrawing = false;
	LineTo(hdcmem, xInit, yInit);
	HDC hdc = GetDC(hWnd);
	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
}