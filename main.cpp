#include "game.h"
#include <Windows.h>



HRESULT CALLBACK MainEventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				default:
					Game::getInstance().onKeyDown(wParam);
					break;
			}
			break;
		case WM_KEYUP:
			switch (wParam)
			{
				case VK_ESCAPE:
					break;
				default:
					Game::getInstance().onKeyUp(wParam);
					break;
			}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND initializeWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wnd;
	ZeroMemory(&wnd, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hInstance = hInstance;
	wnd.lpfnWndProc = MainEventProc;
	wnd.lpszClassName = "GameplayTest";
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	if (!RegisterClassEx(&wnd))
	{
		return 0;
	}

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	HWND hWnd = CreateWindowEx(0,
		"GameplayTest",
		"GameplayTest",
		WS_POPUP,
		0, 0,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	return hWnd;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdLine, int cmdShow)
{
	HWND hMainWnd = initializeWindow(hInstance);

	int initResult = Game::getInstance().initialize(hMainWnd);
	if (initResult)
	{
		return initResult;
	}

	return Game::getInstance().loop();
}