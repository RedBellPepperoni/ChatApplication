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

		}
		bool TCPClient::Init()
		{
			WSADATA data;
			WORD version = MAKEWORD(2, 2);

			int result = WSAStartup(version, &data);

			if (result != 0)
			{
				printf("Client : Error { Cant start Winsock }");
				return false;
			}

			// Setting Endianness to Big endian
			m_buffer.SetEndian(Buffer::Endianness::BIG_ENDIAN);

			return true;
		}


		void TCPClient::Connect()
		{
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


			// If we successfully connect, send a message to the server providing our Username
			//std::string messageToSend = GetUsername() + " : Has Connected";
			std::string messageToSend = GetUsername();

			m_hasConnected = true;
			SendMsg(messageToSend);

		}

		void TCPClient::SendMsg(const std::string msgText)
		{
			if (msgText.empty() || m_serverSocket == INVALID_SOCKET)
			{
				return;
			}

			

			// getiign teh length for Length prefising
			uint32_t length = msgText.length();

			m_buffer.ClearBuffer();

			// using endian Encoding to Write to the buffer
			m_buffer.WriteUInt32(length);

			m_buffer.WriteString(msgText);
			
			const char* bufferText = m_buffer.Get();

			
			send(m_serverSocket, bufferText, length + 4, 0);

			
		}

		int TCPClient::ReceiveMsg(SOCKET sock, std::string& decodedMessage)
		{
			m_buffer.ClearBuffer();

			// Get the First 4 bytes to get the message length (LENGTH PREFIXING)
			int bytesReceived = recv(sock, m_buffer.Get(), 4, 0);

			if (bytesReceived <= 0)
			{
				return -1;
			}


			// Use Endian Decoding to gather the incoming mesasage Length
			int messageLength = m_buffer.ReadUInt32();

			//Clear the buffer before receiving data. 
			m_buffer.ClearBuffer();

			// Growing the Buffer if needed
			m_buffer.ResizeBuffer(messageLength);

			// Getting the rest of he message
			bytesReceived = recv(sock, m_buffer.Get(), messageLength, 0);

			decodedMessage = std::string(m_buffer.Get(), 0, bytesReceived);

			return bytesReceived;
		}

		void TCPClient::ThreadReceive()
		{
			m_threadRunning = true;

			while (m_threadRunning)
			{
				std::string message;
				int bytesReceived = ReceiveMsg(m_serverSocket, message);

				if (bytesReceived > 0)
				{
					std::cout << message << std::endl;
				}
				else
				{
					std::cout << " Server Doesnt Exist " << std::endl;
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