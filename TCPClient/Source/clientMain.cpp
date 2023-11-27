
#include "TCPClient.h"
#include <iostream>
#include <sstream>


//FanshaweGameEngine::Network::Buffer testBuffer;
bool newUser = false;

void DisplayMainMenu()
{
	


	std::cout << "Select Action : [Press number relating to the action]" << std::endl;
	std::cout << "Press 1 : Connect using exisiting account" << std::endl;
	std::cout << "Press 2 : Create and Connect using a new account" << std::endl;

	int selection;

	std::cin >> selection;

	if (selection == 1)
	{
		newUser = false;
	}
	else if (selection == 2)
	{
		newUser = true;
	}

	else
	{
		std::cout << "Incorrect option Selected" << std::endl;
		DisplayMainMenu();

	}
}

int main(int argc, char* argv)
{

	std::string msg = "a";
	std::string username;

	DisplayMainMenu();


	FanshaweGameEngine::Network::TCPClient client;

	if (client.Init())
	{

		client.Connect(newUser);

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

			
			client.SendMsg(messageToSend, MessageType::ChatMessage);
			
		}


	}



}