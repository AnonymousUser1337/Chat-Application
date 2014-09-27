#include "stdclient.h"
#define START_ERROR -1
#define NET_EVENT 1111
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int Run(HWND hwnd);
bool IsKeyDown(int vk);
void print_line(HWND hwnd, char *Msg);

SOCKET cConnect;
SOCKADDR_IN sockaddr = { 0 };


int len = sizeof(sockaddr);
bool isFirstTime;


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
	return Run(hwnd);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit = NULL;
	HWND hEdit2 = NULL;
	#define Print(msg2) print_line(hwnd, msg2)
	#define Println() Print("\r\n")
	switch (msg)
	{
	case WM_CREATE:


		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL  | ES_MULTILINE | ES_AUTOVSCROLL ,
			0, 0, WIDTH, HEIGHT - 65, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL,
			0, 535, WIDTH, 25, hwnd, (HMENU)IDC_MAIN_EDIT2, GetModuleHandle(NULL), NULL);
			SetFocus(hEdit2);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case NET_EVENT:
		if (LOWORD(lParam) == FD_CONNECT)
		{
			Print("You have connected to the server at 127.0.0.1(localhost)");
			Println();
			Print("Enter a Username");
			Println();
			isFirstTime = true;

		}
		if (LOWORD(lParam) == FD_CLOSE)
		{
			Print("The server has shutdown!");
			Println();
		}
		if (LOWORD(lParam) == FD_READ)
		{
			char buff[200];
			if (recv(cConnect, buff, sizeof(buff), NULL) == INVALID_SOCKET)
			{
				Print("Failed to recieve message!");
				Println();
				break;
			}
			Print(buff);
			Println();
		}
		break;
	case WM_COMMAND:
		if (IsKeyDown(VK_RETURN))
		{
			if (isFirstTime)
			{
				char NICK[200] = { 0 };
				
				GetDlgItemText(hwnd, IDC_MAIN_EDIT2, NICK, 200);
				SetDlgItemText(hwnd, IDC_MAIN_EDIT2, "");
				char buffer[200] = {0};
				
				ULONG ipnet = htonl(sockaddr.sin_addr.S_un.S_addr);
				in_addr in;
				in.S_un.S_addr = ipnet;
				strcpy(buffer, inet_ntoa(in));
				
				buffer[strlen(buffer)] = '\n';
				
				int n = strlen(buffer);
				for (int i = 0; i < strlen(NICK); i++)
				{
					buffer[n + i] = NICK[i];
				
				}
				if (send(cConnect, buffer, strlen(buffer) + 1, NULL) == INVALID_SOCKET)
				{
					Print("Failed to send IP Address to the Server!!");
					Println();
					break;
				}
				isFirstTime = false;
			}
			else
			{
				char buffer[200];
				GetDlgItemText(hwnd, IDC_MAIN_EDIT2, buffer, 200);
				SetDlgItemText(hwnd, IDC_MAIN_EDIT2, "");

				if (send(cConnect, buffer, strlen(buffer) + 1, NULL) == INVALID_SOCKET)
				{
					Print("Failed to send IP Address to the Server!!");
					Println();
					break;
				}
			}
		}
		break;
	default:
		return (DefWindowProc(hwnd, msg, wParam, lParam));

	}
}

int Run(HWND hwnd)
{
	MSG msg = { 0 };
	
	WSAData wsa;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsa) != 0) return START_ERROR;
	cConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//create a soccet to connect to the server
	//create a client info struct for the server
	sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(123);
	int len = sizeof(sockaddr);
	WSAAsyncSelect(cConnect, hwnd, NET_EVENT , (FD_CONNECT | FD_CLOSE | FD_READ));
	connect(cConnect, (SOCKADDR*)&sockaddr, len);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
			
		}
		
	}

	return 1;
}

bool IsKeyDown(int vk)
{
	return ((GetAsyncKeyState(vk) & 0x1) != 0) ? true : false;
}

void print_line(HWND hwnd, char *Msg)
{
	const int length = 40000;
	char buff[length];
	GetDlgItemText(hwnd, IDC_MAIN_EDIT, buff, length);
	SetDlgItemText(hwnd, IDC_MAIN_EDIT, strcat(buff, Msg));

}

