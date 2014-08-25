#include "Server.h"
 
Server::Server()
{


}

Server::~Server()
{



}
int Server::run()
{


	WSADATA wsa;
	WORD DllVersion = MAKEWORD(2, 1);

	
	if (initWinSock(wsa, DllVersion)) return START_ERROR;
	//Create two sockets one for listening and one to connect to the clients
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//Create a struct for server information such as ip address and port number that the server is on
	SOCKADDR_IN Server = { 0 };//initialize the server

	Server.sin_family = AF_INET;
	Server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	Server.sin_port = htons(123);
	if(bind(sListen, (SOCKADDR*)&Server, sizeof(Server)) != 0) return BIND_ERROR;
	//listen for a connection with the listening socket and the amount of connections you wan't to allow
	
	
	while (1)
	{
		if(listen_For_Connection(&sConnect, &sListen, &Server)!=0) return LISTEN_ERROR;

		while (1)//Start recieving messages from the client
		{


			char buff[4096] = { 0 };//create a buffer for the message

			if (recv(sConnect, buff, sizeof(buff), NULL) != 0)//only print if there is any message to recieve

			{

				if (buff[0] == 0) break;// if client disconnected then break out of loop and keep listening for connections
				cout << "Message: " << buff << endl;//get the message and print it out
				
			}
			else //if not receiving messages then break out of loop and listen for another connection
				break;
			
				
		}
	}
	//Shutdown server properly
	shutdown(sConnect, 2);
	closesocket(sConnect);
	WSACleanup();
	
	return 0;
}

int Server::listen_For_Connection(SOCKET *sConnect, SOCKET *sListen, SOCKADDR_IN *Server)
{


	//tell the user you are listening for a socket
	
	
	if(listen(*sListen, SOMAXCONN) !=0) return LISTEN_ERROR;


	cout << "Listening for connection" << endl;
	int len = sizeof(*Server);

	
	
	while (1)//loop forever
	{
		if (*sConnect = accept(*sListen, (SOCKADDR*)Server, &len))
		{
			//tell the user that a connection was reached
			cout << "Connection was reached at 127.0.0.1(localhost)" << endl;


			break;//break out of loop

		}
	}
	return 0;

}
