#pragma once
#include <string>
#include <thread>
#include <WS2tcpip.h>
#include "Buffer/Buffer.h"

#pragma comment (lib, "ws2_32.lib")


namespace FanshaweGameEngine
{
	namespace Network
	{
		class TCPClient
		{

		public:

			TCPClient();
			~TCPClient();

			bool Init();
			void Connect();

			void SendMsg(const std::string msgText);
			int ReceiveMsg(SOCKET sock, int& colorAttrib, std::string& username, std::string& decodedMessage);

			std::thread m_clientThread;
			void ThreadReceive();

			std::string GetUsername() const;
			void SetUsername(const std::string newName);
			
			
			bool GetConnected() const;

		private:

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			SOCKET CreateSocket();

			SOCKET m_serverSocket;

			std::string m_serverIp = "127.0.0.1";
			std::string m_username;

			int m_serverPort = 5014;
			bool m_threadRunning = false;

			bool m_hasConnected = true;

			struct addrinfo* info = nullptr;


			int White = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;


			// Default Bugger is using Big Endian
			Buffer m_buffer;
		};
	}
}

