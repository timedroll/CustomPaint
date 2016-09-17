#include "stdafx.h"
#include "Pen.h"

Pen::Pen(HWND hWnd) : ITool(hWnd)
{
	this->hWnd = hWnd;
}


Pen::~Pen()
{
}

void Pen::OnMouseMove(HDC hdcmem, LPARAM lParam)
{
	if (!IsDrawing)
		return;
	MoveToEx(hdcmem, LOWORD(lParam), HIWORD(lParam), NULL);
	LineTo(hdcmem, xInit, yInit);
	xInit = LOWORD(lParam);
	yInit = HIWORD(lParam);
	HDC hdc = GetDC(hWnd);
	RECT rc;
	GetClientRect(hWnd, &rc);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}

void Pen::OnLButtonDown(HDC hdcmem, LPARAM lParam)
{
	IsDrawing = true;
	xInit = LOWORD(lParam);
	yInit = HIWORD(lParam);
}

void Pen::OnLButtonUp(HDC hdcmem, LPARAM lParam)
{
	IsDrawing = false;
}