#ifndef DATABASE_TOOLS_H
#define DATABASE_TOOLS_H
#include <mysql.h>
#include <WinSock.h>
#include <queue>
#include "msg_type.h"


#pragma comment(lib,"wsock32.lib") 
#pragma comment(lib,"libmysql.lib")
class DB_Tool
{
public:
	DB_Tool(string host, string user, string psw, string database, unsigned int port);
	bool DB_Connect();
	void DB_Free_Connect();
	bool DB_Insert(PatientData_t& data);
	bool DB_Read(PatientData_t& data, string uid);
	bool buffer_to_database(queue<PatientData_t>& msg_list);	//��յ�ǰ��Ϣ����

private:
	string host;
	string user;
	string psw;
	string database;
	unsigned int port;
	MYSQL* mysql;	//���ݿ���
	MYSQL_RES* res;	//���ݽ����
};
#endif // !DATABASE_TOOLS_H
