
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

			if (client.GetConnected() && !msg.empty()) 
			{
				//std::ostringstream stream;

				//// Figure something out later about this
				//// Send Just the message

				//stream << client.GetUsername() << ": " << msg;

				//messageToSend = stream.str();


				// Send out the raw message, the username will get handled by the server probably
				messageToSend = msg;

				
			}

			
			client.SendMsg(messageToSend);
			
		}


	}



}