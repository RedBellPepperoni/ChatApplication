#pragma once
#include <string>
#include "Buffer/Buffer.h"


#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")



namespace FanshaweGameEngine
{
	namespace Network
	{

		class TCPServer
		{

		public:

			TCPServer();
			TCPServer(const uint32_t port);
			
			~TCPServer();
			
			// initializes the Server and the Winsock
			bool Init();

			// Run the Server after initialising
			void Run();

			// For some reason SendMessage is defined in windows 
			void SendMsg();


		private:

			SOCKET CreateSocket();

			std::string m_serverIPaddress;
			uint32_t m_serverPort;

			Buffer m_buffer;

			bool isRunning = false;

		};
	}
}

