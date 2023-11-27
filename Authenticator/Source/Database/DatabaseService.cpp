#include "DatabaseService.h"
#include <iostream>

namespace Database
{
	using namespace std;

	void DisplayException(sql::SQLException& e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}


	DatabaseService::DatabaseService()
		: m_Connected(false)
		, m_Connection(nullptr)
		, m_Driver(nullptr)
		, m_ResultSet(nullptr)
		, m_Statement(nullptr)
	{
	}

	DatabaseService::~DatabaseService()
	{
		if (m_Connection != nullptr)
			delete m_Connection;

		if (m_ResultSet != nullptr)
			delete m_ResultSet;

		if (m_Statement != nullptr)
			delete m_Statement;
	}

	bool DatabaseService::ConnectToDatabase(const char* host, const char* username, const char* password, const char* schema)
	{
		if (m_Connected) return true;

		try
		{
			m_Driver = sql::mysql::get_mysql_driver_instance();
			m_Connection = m_Driver->connect(host, username, password);
			m_Statement = m_Connection->createStatement();

			// Set the Schema you want to use.
			m_Connection->setSchema(schema);
		}
		catch (sql::SQLException& e)
		{	
			DisplayException(e);

			printf("Failed to connect to the Database! :( \n");
			return false;
		}

		printf("Successfully connected to the Database! :D \n");

		m_Connected = true;

		return true;

	}


	void DatabaseService::Disconnect()
	{
		if (!m_Connected) return;

		m_Connection->close();

		m_Connected = false;


	}
	sql::PreparedStatement* DatabaseService::PrepareStatement(const char* query)
	{
		return m_Connection->prepareStatement(query);
	}


	// get the actual value if it exists
	sql::ResultSet* DatabaseService::Select(const char* query)
	{
		try 
		{
			m_ResultSet = m_Statement->executeQuery(query);
		}
		catch (sql::SQLException& e)
		{	
			DisplayException(e);
			printf("Error during [Select] Call \n");
			return nullptr;
		}

		return m_ResultSet;
	}

	// Update a preexisting value
	int DatabaseService::Update(const char* query)
	{
		int count = -99;

		try
		{
			 count = m_Statement->executeUpdate(query);
		}
		catch (sql::SQLException& e)
		{
			DisplayException(e);
			printf("Error during [Update] Call \n");
			
		}

		return count;
	}

	// Insert a new Value
	int DatabaseService::Insert(const char* query)
	{
		int count = -99;

		try
		{
			count = m_Statement->executeUpdate(query);
		}
		catch (sql::SQLException& e)
		{
			DisplayException(e);
			printf("Error during [Insert] Call \n");

		}
		return count;
	}
}
