#pragma once
#include <string>
#include <thread>
#include "Auth/AuthService.h"

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
			void Connect(bool newuser);
			


			void SendMsg(const std::string msgText, const MessageType type);
			//int ReceiveMsg(SOCKET sock, int& colorAttrib, std::string& username, std::string& decodedMessage);*/

			std::thread m_clientThread;
			void ThreadReceive();

			std::string GetUsername() const;
			void SetUsername(const std::string newName);
			void SetPassword(const std::string password);
			
			bool GetConnected() const;

		private:

			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			ProtoManager* protocolmanager = nullptr;

			SOCKET CreateSocket();

			SOCKET m_serverSocket;

			std::string m_serverIp = "127.0.0.1";
			std::string m_username;
			std::string m_password;

			int m_serverPort = 5014;
			bool m_threadRunning = false;

			bool m_hasConnected = true;

			struct addrinfo* info = nullptr;


			int White = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;

			
		};
	}
}

