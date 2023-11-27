#pragma once
#include "ProtoManager.h"
#include "Protocol/auth.pb.h"
#include "Database/DatabaseService.h"


class AuthService
{
public:
	AuthService();
	~AuthService();

	void SetupService();

	bool AuthenticateUser(SOCKET sock, const std::string& data);

	
	

};

