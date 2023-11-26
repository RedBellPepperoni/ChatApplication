#pragma once
#include "Protocol/packet.pb.h"
#include <WS2tcpip.h>
#include "Buffer/Buffer.h"


#pragma comment (lib, "ws2_32.lib")



using namespace FanshaweGameEngine::Network;

class ProtoManager
{

public:

	enum class MessageType : uint8_t
	{
		CreateAccount		= 0,
		CreateSuccess		= 1,
		CreateFailure		= 2,
		Authenticate		= 3,
		AuthenticateSuccess = 4,
		AuthenticateFailure = 5,
		ChatMessage			= 6
		
	};


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
		case Blue: return FOREGROUND_BLUE;
			break;
		case Green: return FOREGROUND_GREEN;
			break;
		case BlueGreen: return (FOREGROUND_BLUE | FOREGROUND_GREEN);
			break;
		case Red: return FOREGROUND_RED;
			break;
		case Purple: return (FOREGROUND_RED | FOREGROUND_BLUE);
			break;
		case Yellow:  return (FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case LightYellow: return (Color::Yellow | FOREGROUND_INTENSITY);
			break;
		case LightBlue: return (Color::Blue | FOREGROUND_INTENSITY);
			break;
		case LightRed: return (Color::Red | FOREGROUND_INTENSITY);
			break;
		case Magenta:return (Color::Purple | FOREGROUND_INTENSITY);
			break;
		case LightGreen: return (Color::Green | FOREGROUND_INTENSITY);
			break;
		case White:  return (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
			break;
		case Grey: return  FOREGROUND_INTENSITY;
			break;
		default: return Color::Red | Color::Blue | Color::Green;
			break;
		}
	}


	struct TCPPacket
	{
		MessageType type;
		std::string userName;
		Color color;
		std::string message;
	};

	ProtoManager();
	~ProtoManager();

	


	bool Serialize(std::string& messageToSerialize);
	bool Deserialize(std::string& messageToDeserialize);

	bool SendProtoMessage(SOCKET sock, const TCPPacket& packetdata);
	bool ReceiveProtoMessage(SOCKET sock, TCPPacket& receivedData);

private:

	Buffer m_buffer;

};

