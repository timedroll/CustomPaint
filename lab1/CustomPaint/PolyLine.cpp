#include "stdafx.h"
#include "PolyLine.h"


PolyLine::PolyLine(HWND hWnd) : ITool(hWnd)
{
	this->hWnd = hWnd;
}


PolyLine::~PolyLine()
{
}

void PolyLine::OnMouseMove(HDC hdcmem, LPARAM lParam)
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
	LineTo(hdc, xPrev, yPrev);
	ReleaseDC(hWnd, hdc);
}

void PolyLine::OnLButtonDown(HDC hdcmem, LPARAM lParam)
{
	if (IsDrawing)
	{
		MoveToEx(hdcmem, LOWORD(lParam), HIWORD(lParam), NULL);
		IsDrawing = false;
		LineTo(hdcmem, xPrev, yPrev);
		HDC hdc = GetDC(hWnd);
		RECT rc;
		GetClientRect(hWnd, &rc);
		BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
	}
	IsDrawing = true;
	xInit = xPrev = LOWORD(lParam);
	yInit = yPrev = HIWORD(lParam);
}

void PolyLine::OnLButtonUp(HDC hdcmem, LPARAM lParam)
{
	
}

void PolyLine::OnRButtonDown(HDC hdcmem, LPARAM lParam)
{
	if (IsDrawing)
	{
		HDC hdc = GetDC(hWnd);
		RECT rc;
		GetClientRect(hWnd, &rc);
		BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
	}
	IsDrawing = false;
}
