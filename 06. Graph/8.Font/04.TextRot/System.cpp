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
	m_applicationName = TEXT("Font");

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
	lstrcpy(lf->lfFaceName, TEXT("궁서"));
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
		for (int i = -500; i < 600; i += 100) {
			MyFont = CreateFontIndirect(&lf);
			lf.lfEscapement = i;
			OldFont = (HFONT)SelectObject(hdc, MyFont);
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 150, 250, str, lstrlen(str));
			SelectObject(hdc, OldFont);
			DeleteObject(MyFont);
		}
		SelectObject(hdc, OldBrush);
		DeleteObject(MyBrush);
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