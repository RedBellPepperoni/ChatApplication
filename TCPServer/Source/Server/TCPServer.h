#pragma once
#include <string>
#include <map>

#include "Auth/AuthService.h"


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
			void SendMsg(SOCKET sock, const ProtoManager::TCPPacket& packetdata);





		private:

			SOCKET CreateSocket();

			Color GetUnsedColor();

			int ReceiveMsg(SOCKET socket, std::string& decodedmessage);


			std::string m_serverIPaddress;
			uint32_t m_serverPort;

			

			// Check to make sure the server is running
			bool isRunning = false;


			// Data Storage to determine the data pertaining to the client
			std::map<SOCKET, ClientData> m_clientMap;

			// handle to teh current terminal
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			// Total number of clients that can connect before we refuse any other connections
			const int m_maxClients = 10;

			
			// Conatiner to chekc if a chat color is in use (10 base colors and 1 extra incase needed)
			std::map<Color, bool> m_ColorChart =
			{  
				{Color::Yellow		, false},
				{Color::Blue		, false},
				{Color::Green		, false},
				{Color::Red			, false},
				
				{Color::Purple		, false},
				{Color::LightYellow , false},
				
				{Color::LightBlue	, false},
				{Color::LightRed	, false},
				{Color::BlueGreen	, false},
				{Color::Magenta		, false},
				
				{Color::LightGreen	, false}

			};




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

