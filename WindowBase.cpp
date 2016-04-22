#include<Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass= TEXT("First"); //Ŭ����

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	
	WndClass.cbClsExtra = 0;										//���࿵��1, Ư���� ��� ���Ǹ� �׿ܴ� 0����
	WndClass.cbWndExtra = 0;										//���࿵��2, ���� ����
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);   //�������� ��� ������ ����.
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//���콺 Ŀ�� ����.
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ ����.
	WndClass.hInstance = hInstance;									//������ Ŭ������ ����ϴ� ���α׷��� ��ȣ�� WinMain�� hInstance�� �ָ� �ȴ�.
	WndClass.lpfnWndProc = WndProc;									//�޼����� ó���� �Լ�.(�߿�!)
	WndClass.lpszClassName = lpszClass;								//���α׷� Ŭ������ �̸� ���.
	WndClass.lpszMenuName = NULL;									//���α׷��� ����� �޴��� �����Ѵ�.
	WndClass.style = CS_HREDRAW | CS_VREDRAW;						//������ ��Ÿ��(�����찡 � ���¸� ���� ���� ����)
	RegisterClass(&WndClass);										//WndClass Ư���� ����.

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}