#include "System.h"
#include"resource.h"
SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass & ref)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	if (!InitializeWindows())
		return false;
	return true;
}

void SystemClass::Run()
{
	MSG Message;
	bool done = false;
	ZeroMemory(&Message, sizeof(Message));

	while (!done)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			//메세지가 종료 일시 반복문을 탈출. 해당 처리가 없으면 WndProc에서 창을 없애도 프로그램은 남아있게 된다.
			if (Message.message == WM_QUIT)
				done = true;
			else { // 아닐시 메세지 처리.
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
		else
			Frame();// 메세지가 없어도 계속 처리해야할 함수.
	}
}

void SystemClass::Shutdown()
{
}

bool SystemClass::InitializeWindows()
{
	// WndProc에서 해당 객체로 접근 가능하게 static 포인터에 해당 객체를 넘김.
	ApplicationHandle = this;

	WNDCLASS WndClass;

	// 프로그램 이름 설정.
	m_applicationName = TEXT("RopMod");

	// 현재 프로그램의 인스턴스 값을 가져옵니다.
	m_hInstance = GetModuleHandle(NULL);

	// 윈도우 설정.
	WndClass.cbClsExtra = 0;					//예약영역1, 특수한 경우 사용되며 그외는 0으로
	WndClass.cbWndExtra = 0;					//예약영역2, 이하 동일
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //윈도우의 배경 색상을 지정.
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);			//마우스 커서 지정.
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);		//아이콘 지정.
	WndClass.hInstance = m_hInstance;					//윈도우 클래스를 등록하는 프로그램의 번호로 WinMain의 hInstance를 주면 된다.
	WndClass.lpfnWndProc = WndProc;					//메세지를 처리할 함수.(중요!)
	WndClass.lpszClassName = m_applicationName;				//프로그램 클래스의 이름 등록.
	WndClass.lpszMenuName = NULL;					//프로그램이 사용할 메뉴를 지정한다.
	WndClass.style = CS_HREDRAW | CS_VREDRAW;			//윈도우 스타일(윈도우가 어떤 형태를 갖을 지의 값들)
	RegisterClass(&WndClass);					//WndClass 특성을 저장.

	// 윈도우 생성.
	m_hwnd = CreateWindow(m_applicationName, m_applicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hInstance, NULL);

	// 윈도우 표시.
	ShowWindow(m_hwnd, SW_SHOW);

	return true;
}

bool SystemClass::Frame()
{
	return false;
}

LRESULT SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;

	static int sx, sy, oldx, oldy;
	static bool bNowDraw = false;

	switch (iMessage) {
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
		RECT rect;
		GetClientRect(hWnd, &rect);
		StretchBlt(hdc, 10, 10, rect.right-20, rect.bottom-20, MemDC, 0, 0, 4032, 3024, SRCCOPY);
		SelectObject(MemDC, OldBitmap);
		DeleteObject(MyBitmap);
		DeleteObject(MemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			return 0;  
		}
	case WM_LBUTTONDOWN:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		oldx = sx;
		oldy = sy;

		// Buttondown position dot fix.
		hdc = GetDC(hWnd);
		MoveToEx(hdc, sx, sy, NULL);
		LineTo(hdc, oldx, oldy);
		ReleaseDC(hWnd, hdc);
		bNowDraw = true;
		return 0;
	case WM_MOUSEMOVE:
		if (bNowDraw) {
			hdc = GetDC(hWnd);
			SetROP2(hdc, R2_NOT); // R2_NOT : Black & White good, R2_NOTXORPEN : Color pen good.

			MoveToEx(hdc, sx, sy, NULL);
			LineTo(hdc, oldx, oldy);

			oldx = LOWORD(lParam);
			oldy = HIWORD(lParam);

			MoveToEx(hdc, sx, sy, NULL);
			LineTo(hdc, oldx, oldy);

			ReleaseDC(hWnd, hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw = false;
		hdc = GetDC(hWnd);
		MoveToEx(hdc, sx, sy, NULL);
		LineTo(hdc, oldx, oldy);
		ReleaseDC(hWnd, hdc);
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK SystemClass::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hWnd, iMessage, wParam, lParam);
	}
}