#include "Client.h"
unsigned int __stdcall ClientThread(void*data);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int Run(HWND *hwnd);
Client * c = new Client;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	WNDCLASSEX	wcex = { 0 };
	MSG			msg;
	HWND		hwnd = NULL;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hInstance = hInstance;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClassEx(&wcex))
	{
		MessageBoxA(NULL, "Failed to register class", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}
	hwnd = CreateWindow(CLASS_NAME //Name of the window class
		, CLASS_NAME//Title of the window
		, WS_OVERLAPPEDWINDOW// the window style
		, 0 //x Postition of the window
		, 0// y position of the windpo
		, WIDTH, HEIGHT, // Width and Height
		NULL,//Parent window(we have no parent window)
		NULL,//Menu(we are not using menu's)
		hInstance,//application handle
		NULL);//Creates the window
	if (!hwnd)
	{
		MessageBoxA(NULL, "Failed to register class", "Error", MB_OK | MB_ICONERROR);
		return -2;
	}
	ShowWindow(hwnd, nCmdShow);
	Run(&hwnd);
	return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit = NULL;
	HWND hEdit2 = NULL;
	switch (msg)
	{
	case WM_CREATE:


		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL  | ES_MULTILINE | ES_AUTOVSCROLL ,
			0, 0, WIDTH, HEIGHT - 65, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL,
			0, 535, WIDTH, 25, hwnd, (HMENU)IDC_MAIN_EDIT2, GetModuleHandle(NULL), NULL);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:



		/*HWND hEdit;
		RECT rcClient;

		GetClientRect(hwnd, &rcClient);

		hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		*/
		//UpdateWindow(hEdit);
		//UpdateWindow(hEdit2);
		return (DefWindowProc(hwnd, msg, wParam, lParam));

	}
}

int Run(HWND *hwnd)
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			_beginthreadex(0, 0, ClientThread, (void*)hwnd, 0, 0);
			
		}


	}

	return 1;
}
unsigned int __stdcall ClientThread(void*data)
{

	HWND *hwnd = (HWND*)data;

	
	c->Run(hwnd);
	return 0;
}
