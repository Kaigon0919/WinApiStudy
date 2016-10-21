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
	m_applicationName = TEXT("MyApi");

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
	switch (iMessage) {
	case WM_CREATE:
		lstrcpy(str, TEXT("왼쪽 버튼을 눌러주세요."));
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 0:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("안녕 친구야"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 1:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("열심히 시험 공부 하고 있어?"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 2:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("나는 열심히 개인공부하고 있어"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 3:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("이거 생각보다 잘 만들지 않았어?"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 4:
			KillTimer(hWnd, count++);
			lstrcpy(str, TEXT("아무튼 시험공부 힘내고 화이팅!"));
			InvalidateRect(hWnd, NULL, TRUE);
			SetTimer(hWnd, count, 3000, NULL);
			break;
		case 5:
			KillTimer(hWnd, count);
			lstrcpy(str, TEXT("왼쪽 버튼을 눌러주세요."));
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
		lstrcpy(str, TEXT("왼쪽 버튼을 눌렀습니다."));
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