#include "Client.h"


int main(int argc, int **argv)
{

	Client *client = new Client;
	if (client->run() == START_ERROR)
	{
		MessageBoxA(NULL, "WSA Startup failure", "Error", NULL);

	}
	delete client;
	system("pause");
	return 0;
}
