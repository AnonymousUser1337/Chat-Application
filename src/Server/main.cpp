#include "stdserver.h"
#include <string>
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void print_line(HWND hwnd, const char *Msg);
int Run(HWND );
unsigned int __stdcall receiveMSGThread(void *data);
#define START_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3

struct User
{


	char NICK[200];
	char IP_ADDR[200];
	HWND hwnd;
	SOCKET s;
};

bool isFirstTime = true;
List::ArrayList<User> Users = List::ArrayList<User>();//create a list of users so we can accept as many people as we wan't
SOCKET sListen;
SOCKADDR_IN Server = { 0 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)               /* Controls how the window is to be shown. */
{
	WNDCLASSEX	wcex = {0};
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
		MessageBoxA(NULL,"Failed to register class", "Error", MB_OK | MB_ICONERROR);
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
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	
	return Run(hwnd);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hEdit = NULL;
	int len = sizeof(Server);
	static int i = 0;
	switch (msg)
	{
	case WM_CREATE:


		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			0, 0, WIDTH, HEIGHT, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		
		break;
	case 1111:
		
		if (LOWORD(lParam) == FD_ACCEPT)
		{
			User u = {0};
		
			
			SOCKET s = accept(sListen, (SOCKADDR*)&Server, &len);
			
			if (s == INVALID_SOCKET)
			{
				print_line(hwnd, "Failed to accept the connection!!");
				break;
			}
			u.s = s;

			Users.add(u);
			
			
			
			print_line(hwnd, "IT WAS ACCEPTED!!!!!!!!\r\n");
			isFirstTime = true;
		}
		if (LOWORD(lParam) == FD_CLOSE)
		{
			print_line(hwnd, "Client left the server!\r\n");
			int Counter = 0;
			for (int j = 0; Users.get(j).s != wParam; j++)
			{
				Counter++;
			}
			closesocket(Users.get(Counter).s);
		}
		if (LOWORD(lParam) == FD_READ)
		{
			if (isFirstTime)
			{

				SOCKET temp = Users.get(i).s;
				User u;
				u.s = temp;
				char buff[200] = { 0 };//initialize the pointers to avoid undefined behavior
				if (recv(u.s, buff, sizeof(buff), NULL) == INVALID_SOCKET)//get the IP address
				{
					print_line(hwnd, "Failed to recieve IP Address from socket!");
					print_line(hwnd, "\r\n");
				}
				//print_line(hwnd, buff);
				
				int j = 0;
				ZeroMemory(u.IP_ADDR, sizeof(u.IP_ADDR));
				ZeroMemory(u.NICK, sizeof(u.NICK));
				while (buff[j] != '\n')
				{
					u.IP_ADDR[j] = buff[j];
					j++;
				}
				u.IP_ADDR[strlen(u.IP_ADDR)] = '\0';
				j++;

				int k = 0;
				for (int i = j; i < strlen(buff); i++)
				{
					u.NICK[k]= buff[i];
					k++;
				}
				u.NICK[strlen(u.NICK)] = '\0';
				u.hwnd = hwnd;
				Users.set(i, u);
				
				print_line(hwnd, "Username: ");
				print_line(hwnd, Users.get(i).NICK);
				print_line(hwnd, "\r\n");
				print_line(hwnd, "IP Address: ");
				print_line(hwnd, Users.get(i).IP_ADDR);
				print_line(hwnd, "\r\n");
				isFirstTime = false;
				i++;
				
			}
			else if (isFirstTime == false)
			{
				int Counter = 0;
				int j = 0;
				for (j = 0; Users.get(j).s != wParam; j++)
				{
					Counter++;
				}
				char buff[200] = { 0 };
				char buffer[200] = { 0 };
			
				if (recv(Users.get(Counter).s, buff, sizeof(buff), NULL) == INVALID_SOCKET)//get the IP address
				{
					print_line(Users.get(Counter).hwnd, "Failed to recieve Message from socket!");
					print_line(Users.get(Counter).hwnd, "\r\n");
					break;
				}
				for (int j = 0; j < strlen(Users.get(Counter).NICK); j++)
				{
					buffer[j] = Users.get(Counter).NICK[j];
				}

				//concactonate the message with the username and the ip address
				strcat(buffer, " : ");
				strcat(buffer, buff);
				strcat(buffer, " ,IP Address : ");
				strcat(buffer, Users.get(Counter).IP_ADDR);
				//send the message
				for (int j = 0; j < Users.size(); j++)
				{
					
					if (send(Users.get(j).s, buffer, strlen(buffer) + 1, NULL) == INVALID_SOCKET)
					{
						print_line(Users.get(Counter).hwnd, "Failed to send message to socket!");
						print_line(Users.get(Counter).hwnd, "\r\n");

					}
				}

				
				
			}
		}
		break;
	default:



		HWND hEdit;
		RECT rcClient;

		GetClientRect(hwnd, &rcClient);

		hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);


		return (DefWindowProc(hwnd, msg, wParam, lParam));

	}
}

int Run(HWND hwnd)
{
	MSG msg = { 0 };
	WSADATA wsa;
	WORD DllVersion = MAKEWORD(2, 1);


	if (WSAStartup(DllVersion, &wsa)) return START_ERROR;
	//Create two sockets one for listening and one to connect to the clients
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//List::ArrayList<SOCKET> Connections = List::ArrayList<SOCKET>();//create a list of sockets so we can accept as many people as we wan't

	//Create a struct for server information such as ip address and port number that the server is on
	
	Server.sin_family = AF_INET;
	Server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Server.sin_port = htons(123);
	

	if (bind(sListen, (SOCKADDR*)&Server, sizeof(Server)) == SOCKET_ERROR)
	{
		MessageBoxA(hwnd, "Bind Error","Error", NULL);
		return BIND_ERROR;
	}
	WSAAsyncSelect(sListen, hwnd, 1111, (FD_ACCEPT | FD_CLOSE | FD_READ));
	
	listen(sListen, SOMAXCONN);


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

void print_line(HWND hwnd, const char *Msg)
{
	const int length = 40000;
	char buff[length];
	GetDlgItemText(hwnd, IDC_MAIN_EDIT, buff, length);
	SetDlgItemText(hwnd, IDC_MAIN_EDIT, strcat(buff, Msg));
}
unsigned int __stdcall  receiveMSGThread(void *data)
{


	return 0;
}
