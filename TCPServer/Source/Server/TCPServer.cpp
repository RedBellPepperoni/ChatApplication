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


			// Winsock Intitialized successfully
			printf("WSAStartup succeeded!\n");
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
							send(client, connectionMsg.c_str(), connectionMsg.size() + 1, 0);

							//std::cout << "New user joined the chat." << std::endl;			//Log connection on server side. 

						}

						// ============================= Recieve a Message ===================================
						else
						{

							m_buffer.ClearBuffer();

							// Get the First 4 bytes to get the message length (LENGTH PREFIXING)

							int bytesReceived = recv(sock, m_buffer.Get(), 4, 0);

							int messageLength = m_buffer.ReadUInt32();


							

							//Clear the buffer before receiving data. 
							m_buffer.ClearBuffer();	

							// Growing the Buffer if needed
							m_buffer.ResizeBuffer(messageLength);

							// Getting the rest of he message
							bytesReceived = recv(sock, m_buffer.Get(), messageLength, 0);


							//int bytesReceived = recv(sock, m_buffer.Get(), 512, 0);


							// No message received
							if (bytesReceived <= 0)
							{
								// Close the client connection
								closesocket(sock);
								FD_CLR(sock, &masterDesc);	//Remove connection from file director.
							}

							else
							{

								//Send msg to other clients & not listening socket. 
								// 
								//Loop through the sockets. 
								for (int i = 0; i < masterDesc.fd_count; i++)
								{
									SOCKET outSock = masterDesc.fd_array[i];

									if (outSock != ListenerSocket) {

										if (outSock == sock) {		//If the current socket is the one that sent the message:
											std::string msgSent = "Message delivered.\n";
											send(outSock, msgSent.c_str(), msgSent.size() + 1, 0);	//Notify the client that the msg was delivered. 	
										}
										else {						//If the current sock is not the sender -> it should receive the msg. 
											//std::ostringstream ss;
											//ss << "SOCKET " << sock << ": " << buf << "\n";
											//std::string strOut = ss.str();
											//m_buffer.ClearBuffer();

											send(outSock, m_buffer.Get(), bytesReceived, 0);		//Send the msg to the current socket. 
										}

									}
								}

								

								std::cout << std::string(m_buffer.Get(), 0, bytesReceived) << std::endl;				//Log the message on the server side. 

							}

						}
					}


				}






			




		}
		void TCPServer::SendMsg()
		{
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

	}
}
