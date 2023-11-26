#pragma once
#include "Protocol/auth.pb.h"
#include "ProtoManager.h"


class AuthService
{
public:
	AuthService();
	~AuthService();


	bool AuthenticateUser(SOCKET sock, const std::string& data);


};

