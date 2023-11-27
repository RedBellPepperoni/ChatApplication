#include "TCPClient.h"
#include <iostream>
#include <sstream>



namespace FanshaweGameEngine
{
	namespace Network
	{
		TCPClient::TCPClient()
		{
			m_threadRunning = false;
			protocolmanager = new ProtoManager();

		}
		TCPClient::~TCPClient()
		{
			// Closing the socket
			closesocket(m_serverSocket);
			WSACleanup();

			if (m_threadRunning)
			{
				m_threadRunning = false;
				m_clientThread.join();
				
			}

			delete protocolmanager;

		}
		bool TCPClient::Init()
		{
			if (!protocolmanager->Init())
			{
				return false;
			}

			return true;
		}


		void TCPClient::Connect(bool create)
		{
			std::cout << "Enter your username." << std::endl;

			// get teh Username from the Client
			std::string texmpstring;

			std::cin >> texmpstring;

			SetUsername(texmpstring);

			texmpstring = "";

			std::cout << "Enter your Password." << std::endl;

			std::cin >> texmpstring;

			SetPassword(texmpstring);

			m_serverSocket = CreateSocket();

			// try to connect
			int result = connect(m_serverSocket, info->ai_addr, (int)info->ai_addrlen);

			if (m_serverSocket == SOCKET_ERROR || m_serverSocket == INVALID_SOCKET) {
				printf("Error: Cannot connect to the Server\n");
				freeaddrinfo(info);
				WSACleanup();

				m_hasConnected = false;
				return;
			}


			

			m_hasConnected = true;
			SendMsg(m_password);

		}

		void TCPClient::SendMsg(const std::string msgText)
		{
			ProtoManager::TCPPacket packet;

			packet.color = Color::Grey;
			packet.userName = GetUsername();
			packet.message = msgText;
			packet.type = MessageType::Authenticate;

			protocolmanager->SendProtoMessage(m_serverSocket, packet);

		}

	

		//void TCPClient::SendMsg(const std::string msgText)
		//{
		//	if (msgText.empty() || m_serverSocket == INVALID_SOCKET)
		//	{
		//		return;
		//	}

		//	

		//	// getiign teh length for Length prefising
		//	uint32_t length = msgText.length();

		//	m_buffer.ClearBuffer();

		//	// using endian Encoding to Write to the buffer
		//	m_buffer.WriteUInt32(length);

		//	m_buffer.WriteString(msgText);
		//	
		//	const char* bufferText = m_buffer.Get();

		//	
		//	send(m_serverSocket, bufferText, length + 4, 0);

		//	
		//}

		//int TCPClient::ReceiveMsg(SOCKET sock, int& colorAttrib, std::string& username, std::string& decodedMessage)
		//{
		//	m_buffer.ClearBuffer();


		//	// Message Format from teh server
		//	/*

		//		Message has 5 parts
		//		TotalLength       = [TL] uint32_t
		//		ColorCode         = [C]  uint8_t
		//		Username length   = [UL] uint8_t : usernames shouldn't be more than 256 caracters anyways
		//		UsernameString    = [UStr] string
		//		Messgae		      = [Msg] string
		//		Message Format

		//		 [TL]      [C]       [UL]      [Ustr]
		//	  (4 bytes)  (1 byte)   (1 byte)



		//	*/


		//	// Get the First 4 bytes to get the message length (LENGTH PREFIXING)
		//	int bytesReceived = recv(sock, m_buffer.Get(), 4, 0);

		//	if (bytesReceived <= 0)
		//	{
		//		return -1;
		//	}


		//	// Use Endian Decoding to gather the incoming mesasage Length
		//	int messageLength = m_buffer.ReadUInt32();

		//	//Clear the buffer before receiving data. 
		//	m_buffer.ClearBuffer();

		//	// Growing the Buffer if needed
		//	m_buffer.ResizeBuffer(messageLength);

		//	// Getting the rest of the message
		//	bytesReceived = recv(sock, m_buffer.Get(), messageLength, 0);

		//	// First Element After the refreshed buffer is the ColorAttrib
		//	colorAttrib = m_buffer.Get()[0];

		//	// the Char after teh color is teh length of teh username
		//	int usernameLength = m_buffer.Get()[1];

		//	std::string senderusername(usernameLength, 'x');

		//	// get teh Username
		//	for (int i = 0; i < usernameLength; i++)
		//	{

		//		int offsetIndex = i + 2;
		//		senderusername[i] = m_buffer.Get()[offsetIndex];


		//	}

		//	username = senderusername;



		//	decodedMessage = std::string(m_buffer.Get(), (1+1+usernameLength), bytesReceived - (1 + 1 + usernameLength));

		//	return bytesReceived;
		//}

		void TCPClient::ThreadReceive()
		{
			m_threadRunning = true;

			while (m_threadRunning)
			{
				ProtoManager::TCPPacket packet;
				if (protocolmanager->ReceiveProtoMessage(m_serverSocket, packet))
				{
					SetConsoleTextAttribute(hConsole, GetColorAttrib(packet.color));
					std::cout << packet.userName << " : ";

					SetConsoleTextAttribute(hConsole, White);
					std::cout << packet.message << std::endl;
				}

				else
				{
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
					std::cout << " Server Doesnt Exist " << std::endl;
					SetConsoleTextAttribute(hConsole, White);
					m_threadRunning = false;
					
				}
			}

			

		}

		std::string TCPClient::GetUsername() const
		{
			return m_username;
		}

		void TCPClient::SetUsername(const std::string newName)
		{
			if (!newName.empty())
			{
				m_username = newName;
			}
		}

		void TCPClient::SetPassword(const std::string password)
		{
			if (!password.empty())
			{
				m_password = password;
			}
		}

	

		bool TCPClient::GetConnected() const
		{
			return m_hasConnected;
		}

		SOCKET TCPClient::CreateSocket()
		{
			// Initialize a socket
			SOCKET ListenerSocket = INVALID_SOCKET;

			int result = 0;

			
			struct addrinfo hints;
			ZeroMemory(&hints, sizeof(hints));	// ensure we don't have garbage data 
			hints.ai_family = AF_INET;			// IPv4
			hints.ai_socktype = SOCK_STREAM;	// Stream
			hints.ai_protocol = IPPROTO_TCP;	// TCP
			hints.ai_flags = AI_PASSIVE;


			//result = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &info);
			result = getaddrinfo(m_serverIp.c_str(), std::to_string(m_serverPort).c_str(), &hints, &info);

			// Getting the Details failed
			if (result != 0)
			{
				printf("getaddrinfo failed with error %d\n", result);
				WSACleanup();
				return INVALID_SOCKET;
			}



			// creating a socket with the info provided
			ListenerSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

			// Socket is still invalid, so we just quit 
			if (ListenerSocket == INVALID_SOCKET)
			{
				printf("socket failed with error %d\n", WSAGetLastError());
				freeaddrinfo(info);

				return INVALID_SOCKET;
			}

			


			return ListenerSocket;
		}
	}
}