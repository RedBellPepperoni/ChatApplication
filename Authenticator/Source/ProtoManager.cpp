#include "ProtoManager.h"

ProtoManager::ProtoManager()
{
	m_buffer = new Buffer();
	m_buffer->SetEndian(Buffer::Endianness::BIG_ENDIAN);
}

ProtoManager::~ProtoManager()
{
	WSACleanup();
	delete m_buffer;
}

bool ProtoManager::Init()
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

	return true;
}

int ProtoManager::Serialize(const TCPPacket& packetdata)
{
	m_buffer->ClearBuffer();
	packet::Packet protoPacket;


	protoPacket.set_username(packetdata.userName);
	protoPacket.set_color(packetdata.color);
	protoPacket.set_messagetype((int)packetdata.type);
	protoPacket.set_data(packetdata.message);

	std::string serializedData = protoPacket.SerializeAsString();

	std::cout << "Encoded :" << serializedData << std::endl;

	uint32_t count = serializedData.length();
	std::cout << "Count Before:" << count << std::endl;

	m_buffer->WriteUInt32(count);
	m_buffer->WriteString(serializedData);

	count = count + 4;

	return count;
}

ProtoManager::TCPPacket ProtoManager::Deserialize(std::string messageToDeserialize)
{
	TCPPacket data;

	std::cout << "Recieved :" << messageToDeserialize.length() << std::endl;

	

	packet::Packet protopacket;

	if (!protopacket.ParseFromString(messageToDeserialize))
	{
		printf("Parse Failed");
	}

	data.color = (Color)protopacket.color();
	data.userName = protopacket.username();
	data.type = (MessageType)protopacket.messagetype();
	data.message = protopacket.data();

	return data;
}

bool ProtoManager::SendProtoMessage(SOCKET sock, const TCPPacket& packetdata)
{
	// Dont do anything if this is the case
	if (packetdata.message.empty() || sock == INVALID_SOCKET)
	{
		return false;
	}

	int count  = Serialize(packetdata);

	//std::cout
	
	
	send(sock, m_buffer->Get() , count, 0);

	return true;

}

bool ProtoManager::ReceiveProtoMessage(SOCKET sock, TCPPacket& receivedData)
{
	m_buffer->ClearBuffer();
	 


	// get the first 4 bytes to determine the actual length of the incoming message
	recv(sock, m_buffer->Get(), 4, 0);


	uint32_t msgLength = m_buffer->ReadUInt32();

	
	m_buffer->ResizeBuffer(msgLength);
	m_buffer->ClearBuffer();


	recv(sock, m_buffer->Get(), msgLength, 0);



	std::string data(m_buffer->Get());

	if (data.length() > msgLength)
	{
		data = data.substr(0, msgLength);
	}


	
	if (data.length() <= 0)
	{
		return false;
	}


	receivedData = Deserialize(data);


	return true;
}


