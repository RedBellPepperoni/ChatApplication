
#include "Buffer/Buffer.h"
#include "Server/TCPServer.h"
#include <iostream>

int main(int argc, char* argv)
{
	FanshaweGameEngine::Network::Buffer testBuffer;

	//uint32_t value = 22;
	//testBuffer.WriteUInt32(512, value);
	//uint32_t result = testBuffer.ReadUInt32(512);

	//testBuffer.SetEndian(FanshaweGameEngine::Network::Buffer::Endianness::LITTLE_ENDIAN);

	//if (result != value)
	//{
	//	printf("[TEST FAILED] TestBuffer(1, %d), expected %d but was %d\n", value, value, result);
	//	return 0;
	//}

	//printf(testBuffer.IsBigEndian() ? " BIG ENDIAN" : " LITTLE ENDIAN");

	//printf("[TEST SUCCEEDED] TestBuffer(%d)\n", value);


	// create a TCP server
	FanshaweGameEngine::Network::TCPServer server;

	// Try to Initialize the server (createe and bind socket)
	if (server.Init())
	{
		// Run the server (start listening to multiple clients)
		server.Run();
	}



	



}