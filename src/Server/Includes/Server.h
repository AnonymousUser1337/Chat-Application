#include "stdserver.h"
#define initWinSock(wsa, DllVersion) WSAStartup(DllVersion, &wsa)
#define START_ERROR -1
#define BIND_ERROR -2
#define LISTEN_ERROR -3
class Server
{

public:
	Server();
	~Server();
	int run();
private:
	int listen_For_Connection(SOCKET *sConnect, SOCKET *sListen, SOCKADDR_IN *Server);



};