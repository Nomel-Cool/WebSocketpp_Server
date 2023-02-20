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
	//CSDN害人不浅啊，WTF ? mysql_init(&mysql); 反而是不行的？好像是这样会默认根据已有数据再返回一个有效句柄，而mysql一开始就无效的空的，而指针这种是创建一个新的给你
	mysql = mysql_init(nullptr);  //连接mysql，数据库				
	////返回false则连接失败，返回true则连接成功  
	if (!(mysql_real_connect(mysql, host.c_str(), user.c_str(), psw.c_str(), database.c_str(), port, NULL, 0)))
		//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
	{
		printf("Error connecting to database:%s\n", mysql_error(mysql));	//若没有连接到数据库，就会丢包的，要缓存起来
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
	mysql_close(mysql);		//断开数据库链接
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
	if (mysql_query(mysql, query.c_str())) {        //执行SQL语句 
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
	if (mysql_query(mysql, query.c_str())) {        //执行SQL语句 
		printf("query failed (%s)\n", mysql_error(mysql));
		return false;
	}
	else
	{
		printf("query successed.\n");
	}
	if (!(res = mysql_store_result(mysql)))    //获得sql语句结束后返回的结果集  
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
	//缺失功能：写入失败后，没有导向取bad_write_list,也没有保留失败原因
	//write_flag目前摆设用的，可以写成结构体封装错误信息
	//循环把缓存区清空
	bool write_flag = true;
	while (msg_list.size())
	{
		write_flag = DB_Insert(msg_list.front());
		msg_list.pop();
		printf("The rest of the recieved data: %zd\n", msg_list.size());
	}
	return write_flag;
}