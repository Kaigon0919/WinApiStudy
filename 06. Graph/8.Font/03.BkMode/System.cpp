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
	m_applicationName = TEXT("Font");

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

bool SystemClass::FontSet(LOGFONT * lf)
{
	lf->lfHeight = 50;
	lf->lfWidth = 0;
	lf->lfEscapement = 0;
	lf->lfOrientation = 0;
	lf->lfWeight = 0;
	lf->lfItalic = 0;
	lf->lfUnderline = 0;
	lf->lfStrikeOut = 0;
	lf->lfCharSet = HANGEUL_CHARSET;
	lf->lfOutPrecision = 0;
	lf->lfClipPrecision = 0;
	lf->lfQuality = 0;
	lf->lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lstrcpy(lf->lfFaceName, TEXT("�ü�"));
	return true;
}


LRESULT SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;
	HFONT MyFont, OldFont;
	LOGFONT lf;
	TCHAR * str = TEXT("Go Kaigon");
	switch (iMessage) {
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		MyBrush = CreateHatchBrush(HS_CROSS, RGB(255, 0, 0));
		OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
		Rectangle(hdc, 100, 100, 800, 400);
		FontSet(&lf);
		MyFont = CreateFontIndirect(&lf);
		OldFont = (HFONT)SelectObject(hdc, MyFont);
		SetBkColor(hdc, RGB(255, 0, 0));
		TextOut(hdc, 150, 150, str, lstrlen(str));
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc, 150, 250, str, lstrlen(str));
		SelectObject(hdc, OldBrush);
		SelectObject(hdc, OldFont);
		DeleteObject(MyBrush);
		DeleteObject(MyFont);
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
		return 0;
	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONUP:
		InvalidateRect(hWnd, NULL, true); // SetBkMode reset Test
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK SystemClass::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	default:
		return ApplicationHandle->MessageHandler(hWnd, iMessage, wParam, lParam);
	}
}