#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdarg.h>
#include <d3d9.h>
#include <d3dx9.h>
//#include <d3d.h>

#define WINDOWED

//Main app instances
HINSTANCE g_hInst; //Global inst handle
HWND g_hWnd;

//App window dimensions, type, class and window name
#define WNDWIDTH 1280
#define WNDHEIGHT 720
#define WNDTYPE WS_OVERLAPPEDWINDOW
const char g_szClass[] = "FrameClass";
const char g_szCaption[] = "FrameCaption";

//Entry point
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev,
	LPSTR szCmdLine, int nCmdShow);

//Function to display an error message
void AppError(BOOL Fatal, char *Text, ...);

//Message procedure
LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,
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

	//Obtaining the Direct3D interfaace
	IDirect3D9* g_pD3D;
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {

		std::cout << "Failed init IDirect3D9";
		return 1;
	}

	//Selecting a Disaplay Mode
	D3DDISPLAYMODE d3ddm;

#ifdef WINDOWED
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		std::cout << "Failed init D3DDISPLAYMODE";
		return 1;
	}
#else
	d3ddm.Width = 640;
	d3ddm.Height = 480;
	d3ddm.RefreshRate = 0;
	d3ddm.Format = D3DFMT_R5G6B5;
#endif
	
	//Setting the Presentation Method
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

#ifdef WINDOWED
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
#else
	d3dpp.Windowed = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.BackBufferFormat = d3ddm.Format;
#endif

	d3dpp.BackBufferWidth = WNDWIDTH;
	d3dpp.BackBufferHeight = WNDHEIGHT;

	//Creating the Device Interface and Initializing the Display
	IDirect3DDevice9 *g_pD3DDevice;
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
		&d3dpp, &g_pD3DDevice))) {

		return 1;
	}


	//Creating vertex Buffer
	typedef struct {
		FLOAT x, y, z;
		D3DCOLOR diffuse;
	} sVertex;
#define VertexFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

	IDirect3DVertexBuffer9 *pD3DVB = NULL;

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(
		sizeof(sVertex) * 4, 0, VertexFVF, D3DPOOL_MANAGED, &pD3DVB, NULL))) {

	}

	sVertex Verts[4] = {
		{ -100.0f, 100.0f, 100.0f,	D3DCOLOR_RGBA(255,255,255,255)},
		{ 100.0f,  100.0f, 100.0f,	D3DCOLOR_RGBA(255,0,0,255)	},
		{ 100.0f, -100.0f, 100.0f,	D3DCOLOR_RGBA(0,255,0,255)	},
		{-100.0f, -100.0f, 100.0f,	D3DCOLOR_RGBA(0,0,255,255)	}
	};

	BYTE *PtrVBM;
	if (SUCCEEDED(pD3DVB->Lock(0, 0, (void**)&PtrVBM, 0))) {
		memcpy(PtrVBM, Verts, sizeof(Verts));
		pD3DVB->Unlock();
	}

	//Setting Vertex Stream
	if (FAILED(g_pD3DDevice->SetStreamSource(0, pD3DVB, 0, sizeof(sVertex)))) {
	}

	//Setting Vertex Shader
	if (FAILED(g_pD3DDevice->SetFVF(VertexFVF))) {
	}
	
		//Projection Matrix
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
		if (FAILED(g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj))) {
			std::cout << "Haven't set transform" << std::endl;
		}

		//Setting Material
		D3DMATERIAL9 d3dm;
		ZeroMemory(&d3dm, sizeof(D3DMATERIAL9));

		d3dm.Diffuse.r = d3dm.Ambient.r = 0.0f;
		d3dm.Diffuse.g = d3dm.Ambient.g = 1.0f;
		d3dm.Diffuse.b = d3dm.Ambient.b = 1.0f;
		d3dm.Diffuse.a = d3dm.Ambient.a = 1.0f;

		g_pD3DDevice->SetMaterial(&d3dm);

	if (FAILED(g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 255, 192, 255), 1.0f, 0))) {
	}

	if (SUCCEEDED(g_pD3DDevice->BeginScene())) {
		if (FAILED(g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1))) {
		}

		g_pD3DDevice->EndScene();
	}

	if (FAILED(g_pD3DDevice->Present(NULL, NULL, NULL, NULL))) {
	}
	
	//Main Loop
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
	WNDCLASSEX wcex {};

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

LRESULT FAR PASCAL WindowProc(HWND hWnd, UINT uMsg,
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