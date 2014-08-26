#include "Client.h"


Client::Client()
{


}
Client::~Client()
{




}
int Client::run()
{
	char IP_ADDR[200];
	char NICK[200];
	cout << "Welcome to Basic Client 0.01 Alpha!" << endl;
	cout << "Please enter your IP address" << endl;
	cin.getline(IP_ADDR, sizeof(IP_ADDR));
	cout << "Please enter a nickname" << endl;
	cin.getline(NICK, sizeof(NICK));


	WSAData wsa;
	WORD DllVersion = MAKEWORD(2, 1);
	if(initWinSock(wsa, DllVersion)) return START_ERROR;
	SOCKET cConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//create a soccet to connect to the server
	SOCKADDR_IN sockaddr = {0};//create a client info struct for the server
	sockaddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(123);
	int len = sizeof(sockaddr);
	
	while (1)//look for the server
	{

		if(connect(cConnect, (SOCKADDR*)&sockaddr, len))//if found then break out of loop and go into send msg loop
		{
			cout << "You have connected to the server at 127.0.0.1(localhost)" << endl;
			
			send(cConnect, IP_ADDR, strlen(IP_ADDR) + 1, NULL);
			send(cConnect, NICK, strlen(NICK) + 1, NULL);
			break;
			
			
		}
		


	}

	//send IP address and /nickname


	sendMsgLoop(&cConnect);//send messages to loop until exit
	
	//shutdown client
	shutdown(cConnect, 2);
	closesocket(cConnect);
	WSACleanup();
	return 0;
}



int Client::sendMsgLoop(SOCKET *cConnect)
{

	while (1)
	{

		char buff[4096] = { 0 };//create a buffer for the message/messages we wan't to send

		cout << "Send message: " << endl;//prompt the user for a message
		cin.getline(buff, sizeof(buff));//get the line and store it in the buffer
		if (strcmp(buff, "/exit") == 0)//if the buffer is equal to exit then break out of the loop and end the program
		{

			break;


		}
		
		send(*cConnect, buff, strlen(buff) + 1, NULL);//send the message to the server

	}
	return 0;

}
