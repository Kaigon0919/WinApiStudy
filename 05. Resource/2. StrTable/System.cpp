#include "System.h"
#include "resource.h"

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


	HACCEL hAccel;
	hAccel = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	while (!done)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			//�޼����� ���� �Ͻ� �ݺ����� Ż��. �ش� ó���� ������ WndProc���� â�� ���ֵ� ���α׷��� �����ְ� �ȴ�.
			if (Message.message == WM_QUIT)
				done = true;
			if (!TranslateAccelerator(m_hwnd, hAccel, &Message))
			{
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
	m_applicationName = TEXT("Accelerator & Str_table");

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);					//���α׷��� ����� �޴��� �����Ѵ�.
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
	TCHAR str[256];
	switch (iMessage) {
	case WM_CREATE:
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		LoadString(m_hInstance, IDS_STRING103, str, lstrlen(str));
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			return 0;
		}
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