#include "TCPServer.h"
#include <iostream>
#include "ProtoManager.h"

namespace FanshaweGameEngine
{

	namespace Network
	{

		TCPServer::TCPServer()
			:m_serverIPaddress("127.0.0.1")
			,m_serverPort(5014)
			
		{
			
			protocolmanager = new ProtoManager();

		}

		TCPServer::TCPServer(const uint32_t port)
		{
			m_serverPort = port;
			protocolmanager = new ProtoManager();
		}


		
		TCPServer::~TCPServer()
		{
			WSACleanup();
			delete protocolmanager;
		}


		bool TCPServer::Init()
		{
			

			// Setting Endianness to Big endian
			//m_buffer.SetEndian(Buffer::Endianness::BIG_ENDIAN);

			bool init = protocolmanager->Init();

			if (!init)
			{
				return false;
			}


			// Winsock Intitialized successfully
			SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Green));
			std::cout << "Fanshawe Chat Server is Online!..." << std::endl;
			SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));

			authService.SetupService();

			return true;
		}




		void TCPServer::Run()
		{
			

			// We are ready to start the server 
			isRunning = true;


			// Try to Create a new Valid Socket
			SOCKET ListenerSocket = CreateSocket();


			
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
			ProtoManager::TCPPacket serverData;


			serverData.color = Color::Grey;
			serverData.userName = "Server";

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
						
						if (masterDesc.fd_count > m_maxClients)
						{
							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Red));

							std::cout << "MAX CLIENTS REACHED" << std::endl;
							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));

							//Accept temporality tjust to inform client to try later
							SOCKET client = accept(ListenerSocket, nullptr, nullptr);
							
							//Tell the client that the connection is successful
							serverData.message = "Maximun Users Reached, Try Later!!";
							serverData.type = MessageType::ChatMessage;
							serverData.color = Color::Red;

							// Ackknowlege the connection by sending a welcome message to the client
							protocolmanager->SendProtoMessage(client,serverData);

							closesocket(client);
							// Dont accept any more connections

							continue;							
						}


						

						//Accept and identify a connection 
						SOCKET clientsocket = accept(ListenerSocket, nullptr, nullptr);
	

						// Since this is the first time a client is connectiing, the first message it will send is the Username
						//ReceiveMsg(client, userName);


						ProtoManager::TCPPacket clientData;


						protocolmanager->ReceiveProtoMessage(clientsocket, clientData);
						


						clientData;

						switch (clientData.type)
						{

						case MessageType::CreateAccount:

							authService.CreateAccount(clientsocket, clientData);

							//authService.AuthenticateUser(clientsocket, clientData);

							break;
						
						case MessageType::Authenticate:

							break;

						case MessageType::ChatMessage:

							break;

						default:
							break;
						}



						//auto iterator = m_clientMap.begin();
						//
						//bool usernameAlreadyExists = false;

						//while (iterator != m_clientMap.end())
						//{
						//	if (iterator->second.userName == userName)
						//	{
						//		usernameAlreadyExists = true;
						//		break;
						//	}

						//	iterator++;
						//}

						//if (usernameAlreadyExists)
						//{


						//	//Tell the client that the connection is successful
						//	std::string connectionMsg = "Username Already exists, please join with a different Name";
						//	// Ackknowlege the connection by sending a welcome message to the client
						//	SendMsg(client, serverData);

						//	continue;
						//}


						//Add the connection to the list of sockets
						FD_SET(clientsocket, &masterDesc);


						//Tell the client that the connection is successful
						serverData.message = "Connected to Fanshawe Chat Server!!";
						serverData.type = MessageType::ChatMessage;

						// Ackknowlege the connection by sending a welcome message to the client
						//SendMsg(clientsockt, serverData);

						protocolmanager->SendProtoMessage(clientsocket, serverData);


						
						clientData.color = GetUnsedColor();

						//// Bind teh socket and username in the map
						//m_clientMap.emplace(client, data);

						
						// broadcast message to be sent to All other connected clients
						serverData.message = "[ " + clientData.userName + " has entered the Room!! ]";
						serverData.type = MessageType::ChatMessage;

						// Loop Through all the sockets
						for (uint32_t i = 0; i < masterDesc.fd_count; i++)
						{
							SOCKET outSock = masterDesc.fd_array[i];

							if (outSock != ListenerSocket)
							{
								if (outSock != clientsocket)
								{
										//All other sockest should receive the msg (except the one that is connecting)
										protocolmanager->SendProtoMessage(outSock, serverData);
								}
							}
						}

						SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Grey));
						std::cout<< "           " << serverData.message << std::endl;
						SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));
					}

					//// ============================= Recieve a Message ===================================
					else
					{

						// Get the decodedMessage from the socket
						const char* displaymsg;
						std::string message;
						std::string finalMessage;


						//switch (MessageType)
						//{

						//case MessageType::CreateAccount:

						//	break;
						//case MessageType::CreateSuccess:

						//	break;
						//case MessageType::CreateFailure:

						//	break;
						//case MessageType::Authenticate:

						//	break;
						//case MessageType::AuthenticateSuccess:

						//	break;
						//case MessageType::AuthenticateFailure:

						//	break;
						//case MessageType::ChatMessage:

						//	break;
						//default:
						//	break;
						//}



						// Store the username of the sending socket
						//ProtoManager::TCPPacket data = m_clientMap[sock];




						// Get Client data from Database here

						ProtoManager::TCPPacket data;
						

						// Receive the Length prefixed message and decode it
						int bytesReceived = protocolmanager->ReceiveProtoMessage(sock, data);


						finalMessage = message;

						// No message received ()
						if (bytesReceived <= 0)
						{
							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Grey));
							// Close the client connection
							finalMessage = "[ " + data.userName + " has left the Room!! ]";
							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));

							//// Clear the username and the socket bind
							//m_clientMap.erase(sock);

							// close the disconnected socket
							closesocket(sock);

							//Remove connection from file discriptor
							FD_CLR(sock, &masterDesc);

							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Grey));
							std::cout << "           " << finalMessage << std::endl;
							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));


							for (uint32_t i = 0; i < masterDesc.fd_count; i++)
							{
								SOCKET outSock = masterDesc.fd_array[i];

								if (outSock != ListenerSocket)
								{
                                  	protocolmanager->SendProtoMessage(outSock, serverData);

								}
							}



						}

						else
						{
							// If time permits add user colors and stuff
							SetConsoleTextAttribute(hConsole, GetColorAttrib(data.color));
							std::cout << data.userName << " : ";


							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));

							std::cout << message << std::endl;

							SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));


							for (uint32_t i = 0; i < masterDesc.fd_count; i++)
							{
								SOCKET outSock = masterDesc.fd_array[i];

								if (outSock != ListenerSocket)
								{


									if (outSock == sock)
									{
										//Uncomment this if you want delivery feedback


										////If the current socket is the one that sent the message:
										//std::string msgSent = "Message delivered.";

										//SendMsg(outSock, serverData, msgSent);
									}

									else
									{	//All other sockest should receive the msg 

										protocolmanager->SendProtoMessage(outSock, serverData);
									}


								}


							}


						}


						





						}
					}


				}


			
		}

		//void TCPServer::SendMsg(SOCKET sock, const ProtoManager::TCPPacket& packetdata)
		//{
		//	// Dont do anything if this is the case
		//	if (packetdata.message.empty() || sock == INVALID_SOCKET)
		//	{
		//		return;
		//	}

		//	// geting teh length for Length prefising

		//	// here the Messgae encoding goes like this: 
		//	/*
		//	
		//		Message has 5 parts
		//		TotalLength       = [TL] uint32_t
		//		ColorCode         = [C]  uint8_t
		//		Username length   = [UL] uint8_t : usernames shouldn't be more than 256 caracters anyways
		//		UsernameString    = [UStr] string
		//		Messgae		      = [Msg] string
		//		Message Format

		//		 [TL]      [C]       [UL]      [Ustr]
 	//	      (4 bytes)  (1 byte)   (1 byte)     		



		//	*/

		//	
		//	size_t totallength = 4 + 1 + 1 + packetdata.userName.length() + packetdata.message.length();

		//	//m_buffer.ClearBuffer();


		//	////// Writing thh total length
		//	//m_buffer.WriteUInt32(totallength);

		//	//// Write the Color data at the fifth slot
		//	//m_buffer.Get()[4] = GetColorAttrib(data.clientColor);

		//	//// Write the length of the username at the 6th index [typecasting to char justin case]
		//	//m_buffer.Get()[5] = (char)data.userName.length();

		//	//// Atach the username
		//	//m_buffer.WriteString(6,data.userName);

		//	//// Finally add the actual message
		//	//m_buffer.WriteString(message);

		//	/*const char* bufferText = m_buffer.Get();*/
		//	const char* bufferText = "m_buffer.Get()";


		//	send(sock, bufferText, totallength, 0);

		//}

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

				SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Red));
				printf("socket failed with error %d\n", WSAGetLastError());
				SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));
				freeaddrinfo(info);

				return INVALID_SOCKET;
			}

			// trying to bind the socket
			result = bind(ListenerSocket, info->ai_addr, (int)info->ai_addrlen);
			if (result == SOCKET_ERROR) 
			{
				SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Red));
				printf("bind failed with error %d\n", WSAGetLastError());
				SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));
				closesocket(ListenerSocket);
				freeaddrinfo(info);
				WSACleanup();

				return SOCKET_ERROR;
			}

			


			result = listen(ListenerSocket, SOMAXCONN);

			if (result == SOCKET_ERROR) {
				SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::Red));
				printf("listen failed with error %d\n", WSAGetLastError());
				SetConsoleTextAttribute(hConsole, GetColorAttrib(Color::White));
				closesocket(ListenerSocket);
				freeaddrinfo(info);
				WSACleanup();
				return SOCKET_ERROR;
			}

	
			return ListenerSocket;
		}

		Color TCPServer::GetUnsedColor()
		{
			Color color = Color::White;


			auto iterator = m_ColorChart.begin();

			while (iterator != m_ColorChart.end())
			{
				if (iterator->second == false)
				{
					color = iterator->first;
					break;
				}

				iterator++;
			}
			
			// Making sure we set in use
			m_ColorChart[color] = true;


			return color;
		}

		//int TCPServer::ReceiveMsg(SOCKET socket, std::string& decodedmessage)
		//{
		//	//m_buffer.ClearBuffer();

		//	//// Get the First 4 bytes to get the message length (LENGTH PREFIXING)
		//	//int bytesReceived = recv(socket, m_buffer.Get(), 4, 0);

		//	//// Use Endian Decoding to gather the incoming mesasage Length
		//	//int messageLength = m_buffer.ReadUInt32();

		//	////Clear the buffer before receiving data. 
		//	//m_buffer.ClearBuffer();

		//	//// Growing the Buffer if needed
		//	//m_buffer.ResizeBuffer(messageLength);

		//	//// Getting the rest of he message
		//	//bytesReceived = recv(socket, m_buffer.Get(), messageLength, 0);

		//	//decodedmessage = std::string(m_buffer.Get(), 0, bytesReceived);

		//	//return bytesReceived;

		//	return 45;
		//}

	}
}
