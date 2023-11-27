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

bool AuthService::AuthenticateUser(SOCKET sock, const std::string& data)
{
	return false;
}
