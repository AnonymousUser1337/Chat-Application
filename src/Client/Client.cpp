#include "Client.h"
#include <iostream>
void print_line(HWND hwnd, char *Msg);
Client::Client()
{
}


Client::~Client()
{
}
int Client::Run(HWND *hwnd)
{
	char IP_ADDR[200];
	char NICK[200];
	#define Print(msg) print_line(*hwnd, msg)
    #define Println() Print("\r\n")

	
		Print("Welcome to Basic Client 0.01 Alpha!");
		Println();
		Print("Please enter your IP address");
		Println();
		if (IsKeyDown(VK_RETURN))
		{
			GetDlgItemText(*hwnd, IDC_MAIN_EDIT2, IP_ADDR, sizeof(IP_ADDR));
			SetDlgItemText(*hwnd, IDC_MAIN_EDIT2, "");
			return 0;
		}
		Print("Please enter a nickname");
		Println();
		
		if(IsKeyDown(VK_RETURN))
		{	GetDlgItemText(*hwnd, IDC_MAIN_EDIT2, NICK, sizeof(NICK));
			SetDlgItemText(*hwnd, IDC_MAIN_EDIT2, "");
			return 0;
		}
		//std::cin.getline(NICK, sizeof(NICK));
		
		
	WSAData wsa;
	WORD DllVersion = MAKEWORD(2, 1);
	if (initWinSock(wsa, DllVersion)) return START_ERROR;
	SOCKET cConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//create a soccet to connect to the server
	SOCKADDR_IN sockaddr = { 0 };//create a client info struct for the server
	sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(123);
	int len = sizeof(sockaddr);

	while (1)//look for the server
	{

		if (connect(cConnect, (SOCKADDR*)&sockaddr, len))//if found then break out of loop and go into send msg loop
		{
			Print("You have connected to the server at 127.0.0.1(localhost)");
			Println();
			send(cConnect, IP_ADDR, strlen(IP_ADDR) + 1, NULL);
			send(cConnect, NICK, strlen(NICK) + 1, NULL);
			break;


		}



	}

	//send IP address and /nickname


	//sendMsgLoop(&cConnect, hwnd);//send messages to loop until exit

	//shutdown client
	shutdown(cConnect, 2);
	closesocket(cConnect);
	WSACleanup();
	return 0;
}



int Client::sendMsgLoop(SOCKET *cConnect, HWND *hwnd)
{

	while (1)
	{

		char buff[4096] = { 0 };//create a buffer for the message/messages we wan't to send

		//Print("Send message: ");//prompt the user for a message
		GetDlgItemText(*hwnd, IDC_MAIN_EDIT2, buff, sizeof(buff));
		SetDlgItemText(*hwnd, IDC_MAIN_EDIT2, "");//get the line and store it in the buffer
		if (strcmp(buff, "/exit") == 0)//if the buffer is equal to exit then break out of the loop and end the program
		{

			break;


		}

		send(*cConnect, buff, strlen(buff) + 1, NULL);//send the message to the server

	}
	return 0;

}
void print_line(HWND hwnd, char *Msg)
{
	const int length = 40000;
	char buff[length];
	GetDlgItemText(hwnd, IDC_MAIN_EDIT, buff, length);
	SetDlgItemText(hwnd, IDC_MAIN_EDIT, strcat(buff, Msg));
	
}

bool Client::IsKeyDown(int vk)
{
	return ((GetAsyncKeyState(VK_TAB) & 0x1) != 0)? true:false;
}
