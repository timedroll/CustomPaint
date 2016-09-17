// CustomPaint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "CustomPaint.h"
#include "Commctrl.h"
#include "ITool.h"
#include "Line.h"
#include "Pen.h"
#include "Rect.h"
#include "Ellip.h"
#include "PolyLine.h"
#include <commdlg.h>

#define MAX_LOADSTRING 100



// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

/*HWND CreateToolbar(HWND hWndParent)
{
	const int NumButtons = 2;
	HWND toolbar = NULL;
	HWND hTool = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 0, 0,
		hWndParent, (HMENU)toolbar, GetModuleHandle(NULL), NULL);
	// Send the TB_BUTTONSTRUCTSIZE message, which is required for
	// backward compatibility.
	SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	TBBUTTON tbb[NumButtons];
	TBADDBITMAP tbab;
	//And then we add the standard bitmaps to the toolbar, using the imagelist predefined in the common control library..
	tbab.hInst = HINST_COMMCTRL;
	tbab.nID = IDB_STD_SMALL_COLOR;
	SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tbab);
	//Now that we have our images loaded up, we can add some buttons that use them
	//ZeroMemory(tbb, sizeof(tbb));
#define ID_PENCIL 2
#define ID_LINE 3
	HBITMAP img = (HBITMAP)LoadImage(NULL, L"icons/Pencil", IMAGE_BITMAP, 16, 16, 0);
	//HIMAGELIST himl = ImageList_Create(16, 16, ILC_COLOR4, 2, 2);
	//int imgnum = ImageList_Add(himl, img, NULL);
	tbb[0].iBitmap = 6;
	tbb[0].fsState = TBSTATE_ENABLED;
	tbb[0].fsStyle = TBSTYLE_BUTTON;
	tbb[0].idCommand = ID_PENCIL;

	tbb[1].iBitmap = STD_FILEOPEN;
	tbb[1].fsState = TBSTATE_ENABLED;
	tbb[1].fsStyle = TBSTYLE_BUTTON;
	tbb[1].idCommand = ID_LINE;

	SendMessage(hTool, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON), (LPARAM)&tbb);
	return hTool;
}*/

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.


    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CUSTOMPAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CUSTOMPAINT));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUSTOMPAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CUSTOMPAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
HBITMAP BmpBuffer;
HDC hdcmem, hdcScaled;
ITool *Tool;
RECT rc;
float ScaleIndex = 1;
int PenWidth = 1;
COLORREF PenColor = RGB(0, 0, 0);
POINT PointOfView = { 0,0 };
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }
   HMENU hMenu = CreateMenu();
   HMENU hMenuTool = CreatePopupMenu();
   HMENU hMenuFile = CreatePopupMenu();
   HMENU hMenuColor = CreatePopupMenu();
   HMENU hMenuPenWidth = CreatePopupMenu();

   AppendMenu(hMenu, MF_STRING | MF_POPUP, (int)hMenuFile, L"File");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (int)hMenuTool, L"Tools");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (int)hMenuColor, L"Color");
   AppendMenu(hMenu, MF_STRING | MF_POPUP, (int)hMenuPenWidth, L"Pen Width");
   AppendMenu(hMenuTool, MF_STRING | MF_CHECKED, IDM_PEN, L"Pen");
   AppendMenu(hMenuTool, MF_STRING, IDM_LINE, L"Line");
   AppendMenu(hMenuTool, MF_STRING, IDM_RECT, L"Rectangle");
   AppendMenu(hMenuTool, MF_STRING, IDM_ELLIP, L"Ellipse");
   AppendMenu(hMenuTool, MF_STRING, IDM_POLYLINE, L"Polyline");
   AppendMenu(hMenuFile, MF_STRING, IDM_SAVEAS, L"Save as...");
   AppendMenu(hMenuFile, MF_STRING, IDM_OPEN, L"Open");
   AppendMenu(hMenuColor, MF_STRING, IDM_ChooseBrushColor, L"Choose brush color");
   AppendMenu(hMenuColor, MF_STRING, IDM_ChoosePenColor, L"Choose pen color");
   AppendMenu(hMenuPenWidth, MF_STRING, IDM_PENWIDTH1, L"1");
   AppendMenu(hMenuPenWidth, MF_STRING, IDM_PENWIDTH3, L"3");
   AppendMenu(hMenuPenWidth, MF_STRING, IDM_PENWIDTH5, L"5");
   SetMenu(hWnd, hMenu);
   SetMenu(hWnd, hMenuTool);

   
   //CreateToolbar(hWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   rc;
   rc.bottom = 1080;
   rc.right = 1920;
   rc.left = 0;
   rc.top = 0;
   
   RECT rc1;
   rc1.bottom = rc.bottom * 2;
   rc1.left = rc.left * 2;
   rc1.right = rc.right * 2;
   rc1.top = rc.top * 2;

   HDC hdc = GetDC(hWnd);
   hdcmem = CreateCompatibleDC(hdc);
   
   hdcScaled = CreateCompatibleDC(hdc);
   HBITMAP bmpScaled = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
   SelectObject(hdcScaled, bmpScaled);
   FillRect(hdcScaled, &rc1, CreateSolidBrush(RGB(255, 255, 255)));
   
   SelectObject(hdcmem, (HBRUSH)GetStockObject(NULL_BRUSH));
   BmpBuffer = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
   SelectObject(hdcmem, BmpBuffer);
   FillRect(hdcmem, &rc, CreateSolidBrush(RGB(255, 255, 255)));

   ReleaseDC(hWnd, hdc);

   Tool = new Rect(hWnd);
   SelectObject(hdcmem, CreateSolidBrush(RGB(255, 0, 0)));
   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//


void SetPenColor(HWND hWnd)
{
	CHOOSECOLOR *cc;                 
	cc = (CHOOSECOLOR*)malloc(sizeof(CHOOSECOLOR));
	static COLORREF acrCustClr[16];
	HBRUSH hbrush;                  
	static DWORD rgbCurrent;        
	//ZeroMemory(&cc, sizeof(cc));
	cc->lStructSize = sizeof(CHOOSECOLOR);
	cc->hwndOwner = hWnd;
	cc->lpCustColors = (LPDWORD)acrCustClr;
	cc->rgbResult = rgbCurrent;
	cc->Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(cc) == TRUE)
	{
		SelectObject(hdcmem, CreatePen(PS_SOLID, PenWidth, cc->rgbResult));
		PenColor = cc->rgbResult;
	}
}

void ScaleImage(HWND hWnd, WPARAM wParam)
{
	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		ScaleIndex += 0.2;
	else
		ScaleIndex -= 0.2;
	HDC hdc = GetDC(hWnd);
	SetStretchBltMode(hdcScaled, HALFTONE);
	StretchBlt(hdcScaled, rc.left, rc.top, (rc.right - rc.left) * ScaleIndex, (rc.bottom - rc.top) * ScaleIndex, hdcmem, 0, 0, rc.right, rc.bottom, SRCCOPY);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcScaled, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}

void Pan(HWND hWnd, WPARAM wParam)
{	
	if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0 && PointOfView.x > rc.left)
			PointOfView.x -= 20;
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0 && PointOfView.x < rc.right)
			PointOfView.x += 20;
		else return;
	}
	else
	{
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0 && PointOfView.y > rc.top)
			PointOfView.y -= 20;
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0 && PointOfView.y < rc.bottom)
			PointOfView.y += 20;
		else return;
	}
	HDC hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcScaled, PointOfView.x, PointOfView.y, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}

void SetBrushColor(HWND hWnd)
{
	CHOOSECOLOR *cc;
	cc = (CHOOSECOLOR*)malloc(sizeof(CHOOSECOLOR));
	static COLORREF acrCustClr[16];
	HBRUSH hbrush;
	static DWORD rgbCurrent;
	//ZeroMemory(&cc, sizeof(cc));
	cc->lStructSize = sizeof(CHOOSECOLOR);
	cc->hwndOwner = hWnd;
	cc->lpCustColors = (LPDWORD)acrCustClr;
	cc->rgbResult = rgbCurrent;
	cc->Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(cc) == TRUE)
	{
		SelectObject(hdcmem, CreateSolidBrush(cc->rgbResult));
	}
}

void SaveFileAs(HWND hWnd)
{
	OPENFILENAME ofn;
	char FileName[260] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = (LPWSTR)FileName;
	ofn.nMaxFile = sizeof(FileName);
	ofn.lpstrFilter = (LPCWSTR)L"All\0*.*\0Meta files\0*.emf\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = 0;

	if (GetOpenFileName(&ofn) != TRUE)
		return;
	HDC EmfDC = CreateEnhMetaFile(NULL, ofn.lpstrFile, NULL, NULL);
	BitBlt(EmfDC, 0, 0, 2000, 1000, hdcmem, 0, 0, SRCCOPY);
	HENHMETAFILE hemf = CloseEnhMetaFile(EmfDC);
	DeleteEnhMetaFile(hemf);
}

void OpenFile(HWND hWnd)
{
	OPENFILENAME ofn;
	char FileName[260] = { 0 };

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = (LPWSTR)FileName;
	ofn.nMaxFile = sizeof(FileName);
	ofn.lpstrFilter = (LPCWSTR)L"All\0*.*\0Meta files\0*.emf\0";
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) != TRUE)
		return;
	HENHMETAFILE EnhMetaFile = GetEnhMetaFile(ofn.lpstrFile);
	PlayEnhMetaFile(hdcmem, EnhMetaFile, &rc);
	DeleteEnhMetaFile(EnhMetaFile);
	InvalidateRect(hWnd, &rc, true);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_LBUTTONDOWN:
		Tool->OnLButtonDown(hdcmem, lParam);
		break;
	case WM_LBUTTONUP:
		Tool->OnLButtonUp(hdcmem, lParam);
		break;
	case WM_MOUSEMOVE:
		Tool->OnMouseMove(hdcmem, lParam);
		break;
	case WM_RBUTTONDOWN:
		Tool->OnRButtonDown(hdcmem, lParam);
		break;
	case WM_MOUSEWHEEL:
		if (GET_KEYSTATE_WPARAM(wParam) == MK_CONTROL)
			ScaleImage(hWnd, wParam);
		else
			Pan(hWnd, wParam);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_PEN:
				Tool = new Pen(hWnd);
                break;
			case IDM_LINE:
				Tool = new Line(hWnd);
				break;
			case IDM_RECT:
				Tool = new Rect(hWnd);
				break;
			case IDM_ELLIP:
				Tool = new Ellip(hWnd);
				break;
			case IDM_POLYLINE:
				Tool = new PolyLine(hWnd);
				break;
			case IDM_SAVEAS:
				SaveFileAs(hWnd);
				break;
			case IDM_OPEN:
				OpenFile(hWnd);
				break;
			case IDM_ChooseBrushColor:
				SetBrushColor(hWnd);
				break;
			case IDM_ChoosePenColor:
				SetPenColor(hWnd);
				break;
			case IDM_PENWIDTH1:
				PenWidth = 1;
				SelectObject(hdcmem, CreatePen(PS_SOLID, PenWidth, PenColor));
				break;
			case IDM_PENWIDTH3:
				PenWidth = 3;
				SelectObject(hdcmem, CreatePen(PS_SOLID, PenWidth, PenColor));
				break;
			case IDM_PENWIDTH5:
				PenWidth = 5;
				SelectObject(hdcmem, CreatePen(PS_SOLID, PenWidth, PenColor));
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
			RECT rc;
			GetClientRect(hWnd, &rc);
			BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcmem, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//
// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
