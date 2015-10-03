#include <windows.h>
#include <assert.h>
#include "Graphics.h"
#include "Game.h"

#define QUIT 999

int flag = 0;

Input input;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

		case WM_DESTROY:
			flag = QUIT;
			PostQuitMessage(0);
			return 0;

		case WM_PAINT: 
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

			EndPaint(hwnd, &ps);
        
			return 0;

		case WM_KEYDOWN:
			switch(wParam) {
			case VK_LEFT:
				input.setLeftStatus(true);
				break;
			case VK_RIGHT:
				input.setRightStatus(true);
				break;
			case VK_DOWN:
				input.setDownStatus(true);
				break;
			case VK_UP:
				input.setUpStatus(true);
				break;

			case VK_SHIFT:
				input.setShiftStatus(true);
				break;
			case VK_CONTROL:
				input.setCtrlStatus(true);
				break;
			case VK_SPACE:
				input.setSpaceStatus(true);
				break;
			default:
				if(0x41 <= wParam && wParam <= 0x5A) {
					OutputDebugString(L"Pressed\n");
					input.setKeyStatus(wParam, true);
				}
			}
			return 0;
		case WM_KEYUP:
			switch(wParam) {
			case VK_LEFT:
				input.setLeftStatus(false);
				break;
			case VK_RIGHT:
				input.setRightStatus(false);
				break;
			case VK_DOWN:
				input.setDownStatus(false);
				break;
			case VK_UP:
				input.setUpStatus(false);
				break;

			case VK_SHIFT:
				input.setShiftStatus(false);
				break;
			case VK_CONTROL:
				input.setCtrlStatus(false);
				break;
			case VK_SPACE:
				input.setSpaceStatus(false);
				break;
			default:
				if(0x41 <= wParam && wParam <= 0x5A) {
					OutputDebugString(L"Released\n");
					input.setKeyStatus(wParam, false);
				}
			}
			return 0;
		
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {

	//Register the widow class
	const wchar_t CLASS_NAME[] = L"Pop";

	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);


	//Find the center position for the window
	int border_thickness = GetSystemMetrics(SM_CXSIZEFRAME);

	int sizeX = SCREEN_X + border_thickness * 2;
	int sizeY = SCREEN_Y + border_thickness * 2;

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - sizeX)/2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - sizeY)/2;


    // Create the window.
    HWND winHandle = CreateWindowEx (
        0,											// Optional window styles.
        CLASS_NAME,									// Window class
        L"Prince of Persia (89) Remake",			// Window text
        //WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,	// Window style
		WS_OVERLAPPEDWINDOW,

        // Size and position
        xPos,			//Position X
		yPos,			//Position Y
		sizeX,			//Size X
		sizeY,			//Size Y

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

	//AdjustWindowRectEx(

	assert(winHandle != NULL);

    ShowWindow(winHandle, nCmdShow);

    // Run the message loop.
    MSG msg = { };

	Game game(winHandle, &input);

    while (flag != QUIT) {
		if(PeekMessage(&msg, winHandle, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);


		} else {
			game.GameLoop();
		}
    }

    return 0;
}


