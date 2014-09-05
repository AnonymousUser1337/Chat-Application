#pragma once
#include "stdclient.h"
#define initWinSock(wsa, DllVersion) WSAStartup(DllVersion, &wsa)
#define START_ERROR -1

class Client
{
public:
	Client();
	~Client();
	int Run(HWND *hwnd);
private:
	bool IsKeyDown(int vk);
	int sendMsgLoop(SOCKET *cConnect, HWND *hwnd);
};

