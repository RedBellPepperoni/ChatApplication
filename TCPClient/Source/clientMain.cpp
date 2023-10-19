
#include "TCPClient.h"
#include <iostream>
#include <sstream>

int main(int argc, char* argv)
{
	//FanshaweGameEngine::Network::Buffer testBuffer;

	
	std::string msg = "a";
	std::string username;

	FanshaweGameEngine::Network::TCPClient client;

	std::cout << "Enter your username." << std::endl;

	// get teh Username from the Client
	std::cin >> username;

	client.SetUsername(username);


	if (client.Init())
	{

		client.Connect();

		client.m_clientThread = std::thread([&] {client.ThreadReceive(); });


		

		while (true) 
		{
			// getting the Message from the client terminal, until user presses enter
			std::getline(std::cin, msg);

			std::string messageToSend;

			if (client.GetConnected()) 
			{
				std::ostringstream stream;

				stream << client.GetUsername() << ": " << msg;

				messageToSend = stream.str();

				
			}

			
			client.SendMsg(messageToSend);
			
		}


	}



}