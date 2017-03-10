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
			//�޼����� ���� �Ͻ� �ݺ����� Ż��. �ش� ó���� ������ WndProc���� â�� ���ֵ� ���α׷��� �����ְ� �ȴ�.
			if (Message.message == WM_QUIT)
				done = true;
			else { // �ƴҽ� �޼��� ó��.
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
		else
			Frame();// �޼����� ��� ��� ó���ؾ��� �Լ�.
	}
}

void SystemClass::Shutdown()
{
}

bool SystemClass::InitializeWindows()
{
	// WndProc���� �ش� ��ü�� ���� �����ϰ� static �����Ϳ� �ش� ��ü�� �ѱ�.
	ApplicationHandle = this;

	WNDCLASS WndClass;

	// ���α׷� �̸� ����.
	m_applicationName = TEXT("RopMod");

	// ���� ���α׷��� �ν��Ͻ� ���� �����ɴϴ�.
	m_hInstance = GetModuleHandle(NULL);

	// ������ ����.
	WndClass.cbClsExtra = 0;					//���࿵��1, Ư���� ��� ���Ǹ� �׿ܴ� 0����
	WndClass.cbWndExtra = 0;					//���࿵��2, ���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //�������� ��� ������ ����.
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);			//���콺 Ŀ�� ����.
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);		//������ ����.
	WndClass.hInstance = m_hInstance;					//������ Ŭ������ ����ϴ� ���α׷��� ��ȣ�� WinMain�� hInstance�� �ָ� �ȴ�.
	WndClass.lpfnWndProc = WndProc;					//�޼����� ó���� �Լ�.(�߿�!)
	WndClass.lpszClassName = m_applicationName;				//���α׷� Ŭ������ �̸� ���.
	WndClass.lpszMenuName = NULL;					//���α׷��� ����� �޴��� �����Ѵ�.
	WndClass.style = CS_HREDRAW | CS_VREDRAW;			//������ ��Ÿ��(�����찡 � ���¸� ���� ���� ����)
	RegisterClass(&WndClass);					//WndClass Ư���� ����.

	// ������ ����.
	m_hwnd = CreateWindow(m_applicationName, m_applicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hInstance, NULL);

	// ������ ǥ��.
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