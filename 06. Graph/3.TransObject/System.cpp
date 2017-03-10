#include "System.h"

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
	m_applicationName = TEXT("GdiObject - SolidBrush");

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
	HPEN myPen, oldPen;
	HBRUSH MyBrush, OldBrush;
	
	PAINTSTRUCT ps;

	float persent = 25.0;

	switch (iMessage) {
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		for (int i = 0; i < 250; i += 5)
		{
			MoveToEx(hdc, 0, i, NULL);
			LineTo(hdc, 600, i);
		}
		
		MyBrush = CreateSolidBrush(RGB(0, 255, 0));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		myPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, myPen);
		Ellipse(hdc, 20, 20, 150, 150);

		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, 220, 20, 350, 150);

		SelectObject(hdc, MyBrush);
		SelectObject(hdc, GetStockObject(NULL_PEN));
		Ellipse(hdc, 420, 20, 550, 150);

		//Rectangle(hdc, clientWidth*(persent*0.01), clientHeight*(persent*0.01), clientWidth*(1-0.01*persent), clientHeight*(1 - 0.01 * persent));

		SelectObject(hdc, oldPen);
		SelectObject(hdc, OldBrush);

		DeleteObject(myPen); // �޸𸮿��� ����. Create�� �޸𸮸� �Ҵ�޾� ����Ѵ�.
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		RECT rect;
		GetClientRect(hWnd, &rect);
		clientHeight = rect.bottom;
		clientWidth = rect.right;
		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			return 0;
		}
	case WM_LBUTTONDOWN:
		return 0;
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