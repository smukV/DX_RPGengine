#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

//Main app instances
HINSTANCE g_hInst; //Global inst handle
HWND g_hWnd;

//App window dimensions, type, class and window name
#define WNDWIDTH 640
#define WNDHEIGHT 400
#define WNDTYPE WS_OVERLAPPEDWINDOW
const char g_szClass[] = "FrameClass";
const char g_szCaption[] = "FrameCaption";

//Entry point
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev,
	LPSTR szCmdLine, int nCmdShow);

//Function to display an error message
void AppError(BOOL Fatal, char *Text, ...);

//Message procedure
long FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam);

//Func to register and unregister windows classes
BOOL RegisterWindowClasses(HINSTANCE hInst);
BOOL UnregisterWindowClasses(HINSTANCE hInst);

//Func to create the app's window 
HWND CreateMainWindow(HINSTANCE hInst);

void CenterWindow(HWND hwnd);

//Funcs to init, shutdown, and handle per-frame funcs
BOOL DoInit();
BOOL DoShutDown();
BOOL DoPreFrame();
BOOL DoFrame();
BOOL DoPostFrame();

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev,
	LPSTR szCmdLine, int nCmdShow)
{
	MSG Msg;

	g_hInst = hInst;

	if (RegisterWindowClasses(hInst) == FALSE)
		return FALSE;
	if ((g_hWnd = CreateMainWindow(hInst)) == NULL)
		return FALSE;

	if (DoInit() == TRUE) {
		ZeroMemory(&Msg, sizeof(MSG));
		while (Msg.message != WM_QUIT) {
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			else {
				if (DoPreFrame() == FALSE)
					break;
				if (DoFrame() == FALSE)
					break;
				if (DoPostFrame() == FALSE)
					break;
			}
		}

	}

	DoShutDown();
	UnregisterWindowClasses(hInst);

	return TRUE;
}

BOOL RegisterWindowClasses(HINSTANCE hInst)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = g_szClass;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
		return FALSE;

	return TRUE;
}

BOOL UnregisterWindowClasses(HINSTANCE hInst)
{
	UnregisterClass(g_szClass, hInst);
	return TRUE;
}

HWND CreateMainWindow(HINSTANCE hInst)
{
	HWND hWnd;

	hWnd = CreateWindow(g_szClass, g_szCaption, WNDTYPE, 0, 0,
		WNDWIDTH, WNDHEIGHT, NULL, NULL, hInst, NULL);

	if (!hWnd)
		return NULL;

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	return hWnd;
}

void AppError(BOOL Fatal, char *Text, ...)
{
	char Captiontext[12];
	char ErrorText[2048];
	va_list valist;

	if (Fatal == FALSE)
		strcpy(Captiontext, "ERROR");
	else
		strcpy(Captiontext, "FATAL ERROR");

	va_start(valist, Text);
	vsprintf(ErrorText, Text, valist);
	va_end(valist);

	MessageBox(NULL, ErrorText, Captiontext, MB_OK | MB_ICONEXCLAMATION);

	if (Fatal == TRUE)
		PostQuitMessage(0);
}

long FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:

		CenterWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CenterWindow(HWND hwnd) {

	RECT rc = { 0 };

	GetWindowRect(hwnd, &rc);
	int win_w = rc.right - rc.left;
	int win_h = rc.bottom - rc.top;

	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w) / 2,
		(screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}

BOOL DoInit()
{
	// Perform application initialization functions here
	// such as those that set up the graphics, sound, network, etc.
	// Return a value of TRUE for success, FALSE otherwise.
	return TRUE;
}

BOOL DoShutDown()
{
	// Perform application shutdown functions here
	// such as those that shut down the graphics, sound, etc.
	// Return a value of TRUE for success, FALSE otherwise
	return TRUE;
}

BOOL DoPreFrame()
{
	// Perform pre-frame processing, such as setting up a timer.
	// Return TRUE on success, FALSE otherwise.
	return TRUE;
}

BOOL DoFrame()
{
	// Perform per-frame processing, such as rendering.
	// Return TRUE on success, FALSE otherwise.
	return TRUE;
}

BOOL DoPostFrame()
{
	// Perform post-frame processing, such as time synching, etc.
	// Return TRUE on success, FALSE otherwise.
	return TRUE;
}