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
		enum Color : char
		{
			Yellow, //0
			Blue,
			Green,
			Red,
			Purple,
			LightYellow,
			LightBlue,
			LightRed,
			BlueGreen,
			Magenta,

			LightGreen, // The Extra Color

			White, // Default Message Color
			Grey // Debug Color

		};



		static uint8_t GetColorAttrib(const Color color)
		{
			switch (color)
			{
			case FanshaweGameEngine::Network::Blue: return FOREGROUND_BLUE;
				break;
			case FanshaweGameEngine::Network::Green: return FOREGROUND_GREEN;
				break;
			case FanshaweGameEngine::Network::BlueGreen: return (FOREGROUND_BLUE | FOREGROUND_GREEN);
				break;
			case FanshaweGameEngine::Network::Red: return FOREGROUND_RED;
				break;
			case FanshaweGameEngine::Network::Purple: return (FOREGROUND_RED | FOREGROUND_BLUE);
				break;
			case FanshaweGameEngine::Network::Yellow:  return (FOREGROUND_RED | FOREGROUND_GREEN);
				break;
			case FanshaweGameEngine::Network::LightYellow: return (Color::Yellow | FOREGROUND_INTENSITY);
				break;
			case FanshaweGameEngine::Network::LightBlue: return (Color::Blue | FOREGROUND_INTENSITY);
				break;
			case FanshaweGameEngine::Network::LightRed: return (Color::Red | FOREGROUND_INTENSITY);
				break;
			case FanshaweGameEngine::Network::Magenta:return (Color::Purple | FOREGROUND_INTENSITY);
				break;
			case FanshaweGameEngine::Network::LightGreen: return (Color::Green | FOREGROUND_INTENSITY);
				break;
			case FanshaweGameEngine::Network::White:  return (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
				break;
			case FanshaweGameEngine::Network::Grey: return  FOREGROUND_INTENSITY;
				break;
			default: return Color::Red | Color::Blue | Color::Green;
				break;
			}
		}



		struct ClientData
		{
			Color clientColor;
			std::string userName;
		};


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
			void SendMsg(SOCKET sock, const ClientData data,const std::string message);





		private:

			SOCKET CreateSocket();

			Color GetUnsedColor();

			int ReceiveMsg(SOCKET socket, std::string& decodedmessage);


			std::string m_serverIPaddress;
			uint32_t m_serverPort;

			// The buffer that is used to send and receive and decode/encode data
			Buffer m_buffer;


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

