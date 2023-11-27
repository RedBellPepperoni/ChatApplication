#pragma once
#include "Singleton.h"
#include <mysql\jdbc.h>


namespace Database
{


	class DatabaseService : public Singleton<DatabaseService>
	{
	public:

		DatabaseService();
		~DatabaseService();

		bool ConnectToDatabase(const char* host, const char* username, const char* password, const char* schema);
		void Disconnect();

		sql::PreparedStatement* PrepareStatement(const char* query);
		sql::ResultSet* Select(const char* query);

		int Update(const char* query);
		int Insert(const char* query);

		


	private:

		sql::mysql::MySQL_Driver* m_Driver;
		sql::Connection* m_Connection;
		sql::ResultSet* m_ResultSet;
		sql::Statement* m_Statement;

		bool m_Connected;

	};
}

