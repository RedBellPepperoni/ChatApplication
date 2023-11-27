#include "AuthService.h"

AuthService::AuthService()
{
}

AuthService::~AuthService()
{
}

void AuthService::SetupService()
{
	//Connect to the database
	Database::DatabaseService::GetInstance().ConnectToDatabase("127.0.0.1", "root", "root", "gdp");

	//Database::DatabaseService::GetInstance().Insert();

}

bool AuthService::AuthenticateUser(SOCKET sock, const ProtoManager::TCPPacket data)
{
	//Database

	return false;
}

bool AuthService::CreateAccount(SOCKET sock, const ProtoManager::TCPPacket data)
{

	sql::PreparedStatement* statement = Database::DatabaseService::GetInstance().PrepareStatement("INSERT INTO web_auth(email, salt, hashed_password, userid) VALUES(? , ? , ?, ? )");

	statement->setString(1,data.userName);
	statement->setString(2, "salt");
	statement->setString(3, data.message);
	statement->setInt(4, sock);

	int count = statement->executeUpdate();

	//Database::DatabaseService::GetInstance().Insert();

	return false;
}


