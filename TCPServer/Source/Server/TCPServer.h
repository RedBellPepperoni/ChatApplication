#pragma once
#include <string>
#include "Buffer/Buffer.h"

#include <map>
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
			void SendMsg(SOCKET sock,const std::string message);





		private:

			SOCKET CreateSocket();

			int ReceiveMsg(SOCKET socket, std::string& decodedmessage);

			std::string m_serverIPaddress;
			uint32_t m_serverPort;

			Buffer m_buffer;

			bool isRunning = false;

			std::map<SOCKET, std::string> m_clientMap;

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			// Text Color For the Message
			const int messageAttrib = 
				  FOREGROUND_BLUE
				| FOREGROUND_RED
				| FOREGROUND_GREEN
				| FOREGROUND_INTENSITY;

			// Text Color for the Sender Name
			const int senderAttrib =  
				  FOREGROUND_RED
				| FOREGROUND_GREEN;

			//Text Color for Debug stuff
			const int debugAttrib = FOREGROUND_INTENSITY;

		};
	}
}

