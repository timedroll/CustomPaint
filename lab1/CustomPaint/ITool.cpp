#include "stdafx.h"
#include "ITool.h"


ITool::ITool(HWND hWnd)
{
	this->hWnd = hWnd;
}

ITool::~ITool()
{
}

void ITool::OnRButtonDown(HDC hdcmem, LPARAM lParam)
{
}