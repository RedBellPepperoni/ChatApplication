
#include "Buffer/Buffer.h"
#include "Server/TCPServer.h"


#include <iostream>

int main(int argc, char* argv)
{
	FanshaweGameEngine::Network::Buffer testBuffer;

	// create a TCP server
	FanshaweGameEngine::Network::TCPServer server;

	//// Try to Initialize the server (createe and bind socket)
	if (server.Init())
	{
		// Run the server (start listening to multiple clients)
		server.Run();
	}


	//AuthService authService;

	//authService.SetupService();
	
	


	char* charac;

	std::getchar();
}