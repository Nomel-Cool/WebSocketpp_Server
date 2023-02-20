#include <stdio.h>

#include <iostream>
#include <vector>

#include "Database_Tools.h"

DB_Tool::DB_Tool(string host, string user, string psw, string database, unsigned int port)
{
	this->host = host.c_str();
	this->user = user.c_str();
	this->psw = psw.c_str();
	this->database = database.c_str();
	this->port = port;
}
bool DB_Tool::DB_Connect()
{
	//CSDN���˲�ǳ����WTF ? mysql_init(&mysql); �����ǲ��еģ�������������Ĭ�ϸ������������ٷ���һ����Ч�������mysqlһ��ʼ����Ч�Ŀյģ���ָ�������Ǵ���һ���µĸ���
	mysql = mysql_init(nullptr);  //����mysql�����ݿ�				
	////����false������ʧ�ܣ�����true�����ӳɹ�  
	if (!(mysql_real_connect(mysql, host.c_str(), user.c_str(), psw.c_str(), database.c_str(), port, NULL, 0)))
		//�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		printf("Error connecting to database:%s\n", mysql_error(mysql));	//��û�����ӵ����ݿ⣬�ͻᶪ���ģ�Ҫ��������
		return false;
	}
	else
	{
		printf("Database Connected...\n");
		return true;
	}
}

void DB_Tool::DB_Free_Connect()
{
	mysql_close(mysql);		//�Ͽ����ݿ�����
}

bool DB_Tool::DB_Insert(PatientData_t& data)
{
	string query = "insert into p_list(uid,p_name,d_name,stu_ld,ser_ld) values (";
	query += ("\'" + data.uid + "\',");
	query += ("\'" + data.patientName + "\',");
	query += ("\'" + data.deviceName + "\',");
	query += to_string(data.studyld) + ",";
	query += to_string(data.seriesld) + ");";
	mysql_query(mysql, "set names gbk");
	if (mysql_query(mysql, query.c_str())) {        //ִ��SQL��� 
		printf("query failed (%s)\n", mysql_error(mysql));
		return false;
	}
	else
	{
		printf("query successed.\n");
		return true;
	}
}

bool DB_Tool::DB_Read(PatientData_t& data ,string uid)
{
	string query = "select * from p_list where uid = " + uid;
	mysql_query(mysql, "set names gbk");
	if (mysql_query(mysql, query.c_str())) {        //ִ��SQL��� 
		printf("query failed (%s)\n", mysql_error(mysql));
		return false;
	}
	else
	{
		printf("query successed.\n");
	}
	if (!(res = mysql_store_result(mysql)))    //���sql�������󷵻صĽ����  
	{
		printf("Couldn't get result from %s\n", mysql_error(mysql));
		return false;
	}
	MYSQL_ROW column;
	while (column = mysql_fetch_row(res))
	{
		data.uid = column[0];				//uid
		data.patientName = column[1];		//p_name
		data.deviceName = column[2];		//d_name
		data.studyld = atoi(column[3]);		//stu_ld
		data.seriesld = atoi(column[4]);	//ser_ld
	}
	mysql_free_result(res);
	return true;
}

bool DB_Tool::buffer_to_database(queue<PatientData_t>& msg_list)
{
	//ȱʧ���ܣ�д��ʧ�ܺ�û�е���ȡbad_write_list,Ҳû�б���ʧ��ԭ��
	//write_flagĿǰ�����õģ�����д�ɽṹ���װ������Ϣ
	//ѭ���ѻ��������
	bool write_flag = true;
	while (msg_list.size())
	{
		write_flag = DB_Insert(msg_list.front());
		msg_list.pop();
		printf("The rest of the recieved data: %zd\n", msg_list.size());
	}
	return write_flag;
}