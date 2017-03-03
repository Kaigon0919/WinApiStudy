#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("First"); //클래스

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;										//예약영역1, 특수한 경우 사용되며 그외는 0으로
	WndClass.cbWndExtra = 0;										//예약영역2, 이하 동일
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);   //윈도우의 배경 색상을 지정.
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//마우스 커서 지정.
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//아이콘 지정.
	WndClass.hInstance = hInstance;									//윈도우 클래스를 등록하는 프로그램의 번호로 WinMain의 hInstance를 주면 된다.
	WndClass.lpfnWndProc = WndProc;									//메세지를 처리할 함수.(중요!)
	WndClass.lpszClassName = lpszClass;								//프로그램 클래스의 이름 등록.
	WndClass.lpszMenuName = NULL;									//프로그램이 사용할 메뉴를 지정한다.
	WndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 스타일(윈도우가 어떤 형태를 갖을 지의 값들)
	RegisterClass(&WndClass);										//WndClass 특성을 저장.

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR * str;
	TCHAR strBuffer[128];
	int Score = 85;
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		str = TEXT("BeginPaint");
		TextOut(hdc, 100, 100, str, lstrlen(str));//lstrlen - 문자열의 크기를 반환, 유니코드에 따른 호환성 대비.

		SetTextAlign(hdc, TA_CENTER);//문자열 정렬상태 설정 함수.
		TextOut(hdc, 200, 200, str = TEXT("First"), lstrlen(str));
		TextOut(hdc, 200, 220, str = TEXT("Second"), lstrlen(str));
		TextOut(hdc, 200, 240, str = TEXT("Three"), lstrlen(str));

		//정수를 str 변환, 실수는 swprintf를 사용.
		wsprintf(strBuffer, TEXT("현재 점수는 %d점입니다."), Score);
		TextOut(hdc, 200, 260, strBuffer, lstrlen(strBuffer));
		

		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hWnd);
		str = TEXT("is LButton click");
		TextOut(hdc, 200, 100,str,lstrlen(str));//lstrlen - 문자열의 크기를 반환, 유니코드에 따른 호환성 대비.
		ReleaseDC(hWnd, hdc);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}