#pragma once

#ifndef __SYSTEMCLASS_H_
#define __SYSTEMCLASS_H_
#include <Windows.h>
class SystemClass
{
private:
	HWND m_hwnd;
	LPCTSTR m_applicationName;
	HINSTANCE m_hInstance;

	float clientWidth;
	float clientHeight;
public:
	SystemClass();
	SystemClass(const SystemClass &ref);
	~SystemClass();

	bool Initialize();
	void Run();
	void Shutdown();
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	bool InitializeWindows();
	bool Frame();
	void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
};
static SystemClass * ApplicationHandle = 0;
#endif