#include "stdclient.h"
#define initWinSock(wsa, DllVersion) WSAStartup(DllVersion, &wsa)
#define START_ERROR -1
class Client
{

public:
	Client();
	~Client();
	int run();
private:
	int sendMsgLoop(SOCKET*);



};
