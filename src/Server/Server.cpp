#include "Server.h"
#include <vector>
/*declare these funtions here because functions 
in different threads cannot be declared
as Part of a Class*/
unsigned int __stdcall recieveMsgLoop(void *data);
unsigned int __stdcall recieveMSG(void*data);
void print_line(HWND hwnd, char *Msg);
Server::Server()
{


}

Server::~Server()
{



}
int Server::run(HWND hwnd)
{


	WSADATA wsa;
	WORD DllVersion = MAKEWORD(2, 1);


	if (initWinSock(wsa, DllVersion)) return START_ERROR;
	//Create two sockets one for listening and one to connect to the clients
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	List::ArrayList<SOCKET> Connections = List::ArrayList<SOCKET>();//create a list of sockets so we can accept as many people as we wan't

	for (int i = 0; i < 100; i++)//initialize the Connections properly
	{
		Connections.add(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

	}
	//Create a struct for server information such as ip address and port number that the server is on
	SOCKADDR_IN Server = { 0 };//initialize the server

	Server.sin_family = AF_INET;
	Server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Server.sin_port = htons(123);
	if (bind(sListen, (SOCKADDR*)&Server, sizeof(Server)) != 0) return BIND_ERROR;
	//listen for a connection with the listening socket and the amount of connections you wan't to allow


	struct Params
	{

		List::ArrayList<SOCKET>Connections;
		char *NICK = new char[4096];
		char *IP_ADDR = new char[4096];
		HWND *hwnd;
	};
	Params p;
	p.hwnd = &hwnd;
	int i = 0;
	int len = sizeof(Server);
	print_line(hwnd, "Listening for a Connection\r\n");

	while (1)
	{



		//Listen for connections
		listen(sListen, SOMAXCONN);
		Connections.set(i, accept(sListen, (SOCKADDR*)&Server, &len));
		if (Connections.get(i))//if a connection was reached then tell the user that someone connected to server
		{
			char buff1[4096] = { 0 };//initialize the pointers to avoid undefined behavior
			char buff2[4096] = { 0 };
			recv(Connections.get(i), buff1, sizeof(buff1), NULL);//get the IP address
			recv(Connections.get(i), buff2, sizeof(buff2), NULL);//get the Nickname
			p.Connections = Connections;
			int count = 0;
			for (int j = 0; j < strlen(buff1); j++)
			{

				p.IP_ADDR[j] = buff1[j];

				count++;
			}
			p.IP_ADDR[count] = '\0';//null terminate the string

			count = 0;
			for (int j = 0; j < strlen(buff2); j++)
			{

				p.NICK[j] = buff2[j];

				count++;
			}
			p.NICK[count] = '\0';//null terminate the string
			print_line(hwnd, p.NICK);
			print_line(hwnd, " has joined the server with the IP Address of: ");
			print_line(hwnd, p.IP_ADDR);
			print_line(hwnd, "\r\n");
			

		}


		_beginthreadex(0, 0, recieveMsgLoop, (void*)&p, 0, 0);//begin a new thread so the server can listen for connections while recieving data

		i++;//go to the next Connections
	}

	//Shutdown server properly
	for (i = 0; i < 100; i++)
	{
		shutdown(Connections.get(i), 2);
		closesocket(Connections.get(i));
	}
	WSACleanup();

	return 0;
}


unsigned int __stdcall recieveMsgLoop(void *data)
{
	struct Params
	{

		List::ArrayList<SOCKET>Connections;
		char *NICK = (char*)calloc(4096, sizeof(char));
		char *IP_ADDR = (char*)calloc(4096, sizeof(char));
		HWND *hwnd;
	};
	Params *p = (Params*)data;

	struct Params2//create a struct for paramaters 
	{

		int i;
		List::ArrayList<SOCKET>sConnections;
		char NICK[4096];
		char IP_ADDR[4096];
		HWND *hwnd;
	};

	Params2 p2;
	int count = 0;
	for (int j = 0; j < strlen(p->NICK); j++)
	{
		p2.NICK[j] = p->NICK[j];
		count++;
	}
	p2.NICK[count] = '\0';//null terminate the string
	count = 0;
	for (int k = 0; k < strlen(p->IP_ADDR); k++)
	{
		p2.IP_ADDR[k] = p->IP_ADDR[k];
		count++;
	}
	p2.IP_ADDR[count] = '\0';//null terminate the string
	p2.hwnd = p->hwnd;
	while (1)//only print if there is any message to recieve
	{

		for (int i = 0; i < 100; i++)
		{
			p2.i = i;//get the index
			p2.sConnections = p->Connections;//get the list of Connections
			_beginthreadex(0, 0, recieveMSG, (void*)&p2, 0, 0);//Create a new thread to handle recieving multiple messages at once
		}
	}




	return 0;
}
unsigned int __stdcall recieveMSG(void*data)
{


	struct Params2//create a struct to get all the paramaters passed in
	{

		int i;				//index
		List::ArrayList<SOCKET>sConnections;//Connections
		char NICK[4096];
		char IP_ADDR[4096];
		HWND *hwnd;
	};
	Params2 *p = (Params2*)data;//get the paramaters and store in struct pointer
	char buff[4096] = { 0 };//create a buffer for the message and initialize it to avoid undefine behavior
	if (!recv(p->sConnections.get(p->i), buff, sizeof(buff), NULL))
	{
		print_line(*p->hwnd, "Client has left the Server\r\n");
		
		return 1;//if its not recieving anything return out of function
	}
	if (buff[0] == 0) return 2;//if message is null then dont print the message
	
	print_line(*p->hwnd, p->NICK);
	print_line(*p->hwnd, ": "); 
	print_line(*p->hwnd, buff);
	print_line(*p->hwnd, " ,IP Address: ");
	print_line(*p->hwnd, p->IP_ADDR);
	print_line(*p->hwnd, "\r\n");

	return 0;
}

void print_line(HWND hwnd, char *Msg)
{
	const int length = 2000;
	char buff[length];
	GetDlgItemText(hwnd, IDC_MAIN_EDIT, buff, length);
	SetDlgItemText(hwnd, IDC_MAIN_EDIT, strcat(buff, Msg));
}
