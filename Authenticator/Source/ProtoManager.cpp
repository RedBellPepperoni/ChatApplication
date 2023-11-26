#include "ProtoManager.h"

ProtoManager::ProtoManager()
{
}

ProtoManager::~ProtoManager()
{
}

bool ProtoManager::Serialize(std::string& messageToSerialize)
{
	return false;
}

bool ProtoManager::Deserialize(std::string& messageToDeserialize)
{
	return false;
}

bool ProtoManager::SendProtoMessage(SOCKET sock, const TCPPacket& packetdata)
{
	return false;
}

bool ProtoManager::ReceiveProtoMessage(SOCKET sock, TCPPacket& receivedData)
{
	return false;
}


