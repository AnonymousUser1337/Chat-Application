#include "Server.h"


int main(int argc, char**argv)
{
	Server *server = new Server;
	switch(server->run())//run the server if there was a problem with the server then return the error code -1
	{
		//Handle the errors
	case START_ERROR:
		MessageBoxA(NULL, "WSA Startup Failure", "Error", NULL);
		break;
	case BIND_ERROR:
		MessageBoxA(NULL, "Failed to Bind to Socket", "Error", NULL);
		break;

	case LISTEN_ERROR:
		MessageBoxA(NULL, "Error Listening to socket", "Error", NULL);
		break;


	}
	delete server;
	system("pause");//pause right before ending the program
	return 0;
}


