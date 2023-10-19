#include "TCPServer.h"
#include <iostream>

namespace FanshaweGameEngine
{

	namespace Network
	{

		TCPServer::TCPServer()
			:m_serverIPaddress("127.0.0.1")
			,m_serverPort(5014)
			
		{
			
		}

		TCPServer::TCPServer(const uint32_t port)
		{
			m_serverPort = port;
		}


		
		TCPServer::~TCPServer()
		{
			WSACleanup();
		}


		bool TCPServer::Init()
		{
			// Init thge Winsock
			// Container for the winsock data
			WSADATA data;
			// Version definition
			WORD version = MAKEWORD(2, 2);

			// Initialize and startup the Winsock
			int winsockInit = WSAStartup(version, &data);

			//  The WSAStartup failed miserably :(
			if (winsockInit != 0)
			{
				printf("WSAStartup failed with error %d\n", winsockInit);
				return false;
			}

			// Setting Endianness to Big endian
			//m_buffer.SetEndian(Buffer::Endianness::BIG_ENDIAN);


			// Winsock Intitialized successfully
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "Fanshawe Chat Server is Online!..." << std::endl;
			SetConsoleTextAttribute(hConsole, messageAttrib);

			return true;
		}




		void TCPServer::Run()
		{
			

			// We are ready to start the server 
			isRunning = true;


			// Try to Create a new Valid Socket
			SOCKET ListenerSocket = CreateSocket();


			/*while (isRunning)
			{*/

				if (ListenerSocket == INVALID_SOCKET)
				{
					// Invalid handle, we exit here
					return;
				}


				// A file descriptor that stores all the data for all the sockkets
				fd_set masterDesc;

				// Clean up the masterDesc of garbage values/previous values;
				FD_ZERO(&masterDesc);

				// Binding the Lisner socket to the file descriptor
				FD_SET(ListenerSocket, &masterDesc);



				// Loop
				while (isRunning)
				{
					// Creating a copy, since the descriptor will get cleared everytime
					fd_set copyDesc = masterDesc;
					int socketCount = select(0, &copyDesc, nullptr, nullptr, nullptr);


					// Get msgs only from the specified sockets
					for (int i = 0; i < socketCount; i++)
					{
						


						// Looping through all the scokets that are active
						SOCKET sock = copyDesc.fd_array[i];

						// ============================= Accept a Connection ===================================
						if (sock == ListenerSocket) 
						{

							//Accept and identify a connection 
							SOCKET client = accept(ListenerSocket, nullptr, nullptr);

							//Add the connection to the list of sockets
							FD_SET(client, &masterDesc);

							//Tell the client that the connection is successful
							std::string connectionMsg = "Connected to Fanshawe Chat Server!!\n";
							
							// Ackknowlege the connection by sending a welcome message to the client
							SendMsg(client, connectionMsg);

							// Conatiner to store incoming username
							std::string userName;

							// Since this is the first time a client is connectiing, the first message it will send is the Username
							ReceiveMsg(client, userName);
							
							// Bind teh socket and username in the map
							m_clientMap.emplace(client, userName);

							SetConsoleTextAttribute(hConsole, debugAttrib);
							// broadcast message to be sent to All other connected clients
							std::string joinMsg ="          [ " + userName + " has entered the Chat!! ]";
							SetConsoleTextAttribute(hConsole, messageAttrib);

							// Loop Through all the sockets
							for (uint32_t i = 0; i < masterDesc.fd_count; i++)
							{
								SOCKET outSock = masterDesc.fd_array[i];

								if (outSock != ListenerSocket)
								{
									if (outSock != client)
									{	
										//All other sockest should receive the msg (except the one that is connecting)
										SendMsg(outSock,joinMsg);
									}
								}	
							}

							// Output the messae on teh server terminal
							std::cout << joinMsg << std::endl;
						}

						//// ============================= Recieve a Message ===================================
						else
						{

							// Get the decodedMessage from the socket
							const char* displaymsg;
							std::string message;
							std::string finalMessage;

							// Store the username of the sending socket
							std::string username = m_clientMap[sock];

							// Receive the Length prefixed message and decode it
							int bytesReceived = ReceiveMsg(sock, message);

							// Add the username identifier to teh message before broadcasting
							finalMessage = username + " : " + message;


							// No message received ()
							if (bytesReceived <= 0)
							{
								SetConsoleTextAttribute(hConsole, debugAttrib);
								// Close the client connection
								finalMessage = username + " has left the Chat :(";
								SetConsoleTextAttribute(hConsole, messageAttrib);

								// Clear the username and the socket bind
								m_clientMap.erase(sock);

								// close the disconnected socket
								closesocket(sock);

								//Remove connection from file discriptor
								FD_CLR(sock, &masterDesc);	

								SetConsoleTextAttribute(hConsole, debugAttrib);
								std::cout << "          [ " + username + " has left the Chat!! ]" << std::endl;
								SetConsoleTextAttribute(hConsole, messageAttrib);
							}

							else
							{
								// If time permits add user colors and stuff
								SetConsoleTextAttribute(hConsole, senderAttrib);
								std::cout << username << " : ";

								SetConsoleTextAttribute(hConsole, messageAttrib);
								std::cout << message << std::endl;

							}

						


							for (uint32_t i = 0; i < masterDesc.fd_count; i++)
							{
								SOCKET outSock = masterDesc.fd_array[i];

								if (outSock != ListenerSocket)
								{


									if (outSock == sock)
									{	//If the current socket is the one that sent the message:
										std::string msgSent = "Message delivered.";
										
										SendMsg(outSock, msgSent);
									}

									else
									{	//All other sockest should receive the msg 

										
										SendMsg(outSock, finalMessage);
									}


								}

								//if()
							}

							//std::cout << message << std::endl;				//Log the message on the server side. 

							

						}
					}


				}



		}

		void TCPServer::SendMsg(SOCKET sock, const std::string message)
		{
			if (message.empty() || sock == INVALID_SOCKET)
			{
				return;
			}

			// getiign teh length for Length prefising
			size_t length = message.length();

			m_buffer.ClearBuffer();

			m_buffer.WriteUInt32(length);


			m_buffer.WriteString(message);


			const char* bufferText = m_buffer.Get();


			send(sock, bufferText, length + 4, 0);

		}

		SOCKET TCPServer::CreateSocket()
		{
			// Initialize a socket
			SOCKET ListenerSocket = INVALID_SOCKET;

			int result = 0;

			struct addrinfo* info = nullptr;
			struct addrinfo hints;
			ZeroMemory(&hints, sizeof(hints));	// ensure we don't have garbage data 
			hints.ai_family = AF_INET;			// IPv4
			hints.ai_socktype = SOCK_STREAM;	// Stream
			hints.ai_protocol = IPPROTO_TCP;	// TCP
			hints.ai_flags = AI_PASSIVE;


			//result = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &info);
			result = getaddrinfo(m_serverIPaddress.c_str(), std::to_string(m_serverPort).c_str(), &hints, &info);

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

			// trying to bind the socket
			result = bind(ListenerSocket, info->ai_addr, (int)info->ai_addrlen);
			if (result == SOCKET_ERROR) 
			{
				printf("bind failed with error %d\n", WSAGetLastError());
				closesocket(ListenerSocket);
				freeaddrinfo(info);
				WSACleanup();

				return SOCKET_ERROR;
			}

			


			result = listen(ListenerSocket, SOMAXCONN);

			if (result == SOCKET_ERROR) {
				printf("listen failed with error %d\n", WSAGetLastError());
				closesocket(ListenerSocket);
				freeaddrinfo(info);
				WSACleanup();
				return SOCKET_ERROR;
			}

	
			return ListenerSocket;
		}

		int TCPServer::ReceiveMsg(SOCKET socket, std::string& decodedmessage)
		{
			m_buffer.ClearBuffer();

			// Get the First 4 bytes to get the message length (LENGTH PREFIXING)
			int bytesReceived = recv(socket, m_buffer.Get(), 4, 0);

			// Use Endian Decoding to gather the incoming mesasage Length
			int messageLength = m_buffer.ReadUInt32();

			//Clear the buffer before receiving data. 
			m_buffer.ClearBuffer();

			// Growing the Buffer if needed
			m_buffer.ResizeBuffer(messageLength);

			// Getting the rest of he message
			bytesReceived = recv(socket, m_buffer.Get(), messageLength, 0);

			decodedmessage = std::string(m_buffer.Get(), 0, bytesReceived);

			return bytesReceived;
		}

	}
}
