#include "System.h"

SystemClass::SystemClass() : count(0), isStart(false)
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
	m_applicationName = TEXT("MyApi");

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
	switch (iMessage) {
	case WM_CREATE:
		lstrcpy(str, TEXT("���� ��ư�� �����ּ���."));
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("�ȳ� ģ����"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 1:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("������ ���� ���� �ϰ� �־�?"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 2:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("���� ������ ���ΰ����ϰ� �־�"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 3:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("�̰� �������� �� ������ �ʾҾ�?"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 4:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("�ƹ�ư ������� ������ ȭ����!"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 5:
			KillTimer(hWnd, count);
			lstrcpy(str, TEXT("���� ��ư�� �����ּ���."));
			InvalidateRect(hWnd, NULL, TRUE);
			count = 0;
			isStart = false;
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		return 0;
	case WM_CHAR:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:
		if (isStart)
			break;
		lstrcpy(str, TEXT("���� ��ư�� �������ϴ�."));
		InvalidateRect(hWnd, NULL, TRUE);
		SetTimer(hWnd, count, 3000, NULL);
		isStart = true;
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
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