//#include "WebSocketServer.h"
//#include "Database_Tools.h"
//
//
//int main() {
//	//Server on. Listen on port 9002
//	unsigned short net_port = 9002;
//	WebsocketServer server(net_port);
//
//	//数据库操作，与服务器信息接收业务拆分
//	string host = "localhost";
//	string user = "root";
//	string psw = "123456";
//	string database = "patient_base";
//	unsigned int port = 3306;
//	DB_Tool dbt(host, user, psw, database, 3306);
//	dbt.DB_Connect();
//	try {
//		//创建对象时已经启动了run线程了
//		//可以通过new多个服务端来监听多个端口
//		//但是信息会很乱，不如直接多开个服务器有效多了
//		//命令台用呼出式，不然信息会堆积
//		server.m_command();
//		dbt.buffer_to_database(server.get_msg_list());
//	}
//	catch (websocketpp::exception const& e) {
//		cout << "error: " << e.what() << endl;
//	}
//	dbt.DB_Free_Connect();
//	return 0;
//}




//
//
////int main(int argc, char** argv)
////{
////	MYSQL* mysql;	//数据库句柄
////	MYSQL_RES* res = nullptr;
////	PatientData_t data;
////	data.uid = "114514";
////	data.patientName = "田所浩二";//中文占两个，string自留一个空char
////	data.deviceName = "HuaWei";
////	data.studyld = 111;
////	data.seriesld = 777;
////	//CSDN害人不浅啊，WTF ? mysql_init(&mysql); 反而是不行的？好像是这样会默认根据已有数据再返回一个有效句柄，而mysql一开始就无效的空的，而指针这种是创建一个新的给你
////	mysql = mysql_init(nullptr);  //连接mysql，数据库				
////	bool connect_flag = false;	//链接标志，表示是否有连接上
////	bool save_flag = false;	//存储标志，表示是否insert语句执行成功
////	const char host[] = "localhost";
////	const char user[] = "root";
////	const char psw[] = "123456";
////	const char database[] = "Patient_Base";
////	const int port = 3306;
////	////返回false则连接失败，返回true则连接成功  
////	if (!(mysql_real_connect(mysql, host, user, psw, database, port, NULL, 0)))
////		//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
////	{
////		printf("Error connecting to database:%s\n", mysql_error(mysql));	//若没有连接到数据库，就会丢包的，要缓存起来
////		connect_flag = false;
////	}
////	else
////	{
////		printf("Database Connected...\n");
////		connect_flag = true;
////	}
////	string query = "insert into p_list(uid,p_name,d_name,stu_ld,ser_ld) values (";
////	query += ("\'" + data.uid + "\',");
////	query += ("\'" + data.patientName + "\',");
////	query += ("\'" + data.deviceName + "\',");
////	query += to_string(data.studyld) + ",";
////	query += to_string(data.seriesld) + ");";
////	mysql_query(mysql, "set names gbk");
////	if (mysql_query(mysql, query.c_str())) {        //执行SQL语句 
////		printf("query failed (%s)\n", mysql_error(mysql));
////		save_flag = false;
////	}
////	else
////	{
////		printf("query successed.");
////		save_flag = true;
////	}
////	mysql_close(mysql);	//断开数据库链接
////	return 0;
////}
//




////test
//
////MYSQL_FIELD* fd;  //字段列数组
////char field[32][32];  //存字段名二维数组
////MYSQL_RES* res; //这个结构代表返回行的一个查询结果集
////MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列
////std::string query; //查询语句
////using namespace std;
//////函数声明 
////bool ConnectDatabase();
////void FreeConnect();
////int main(int argc, char** argv)
////{
////
////	MYSQL mysql; //mysql连接
////	//初始化mysql  
////	mysql_init(&mysql);  //连接mysql，数据库  
////	cout << mysql_error(&mysql) << endl;
////	const char host = "localhost";
////	const char user = "root";
////	const char *psw = "123456";
////	const char *database = "Patient_Base";
////	const unsigned int port = 3306;
////	//返回false则连接失败，返回true则连接成功  
////	if (!(mysql_real_connect(&mysql, host, user, psw, database, port, NULL, 0)))
////		//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
////	{
////		printf("Error connecting to database:%s\n", mysql_error(&mysql));
////	}
////	else
////	{
////		printf("Database Connected...\n");
////	}
////	query = "select * from p_list";
////	if (mysql_query(&mysql, query.c_str()))        //执行SQL语句  
////	{
////		printf("Query failed (%s)\n", mysql_error(&mysql));
////	}
////	else
////	{
////		printf("query success\n");
////	}
////	return 0;
////}
////
////
////
////
////
////
//////连接数据库  
////bool ConnectDatabase()
////{
////	
////}
//////释放资源  
////void FreeConnect()
////{
//////	mysql_free_result(res);  //释放一个结果集合使用的内存。
////}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	/////SECURITY//////
//	/*
//	// 原始明文
//	std::string srcText = "this is an example";
//
//	std::string encryptText;
//	std::string encryptHexText;
//	std::string decryptText;
//
//	std::cout << "=== 原始明文 ===" << std::endl;
//	std::cout << srcText << std::endl;
//
//	// md5
//	std::cout << "=== md5哈希 ===" << std::endl;
//	md5(srcText, encryptText, encryptHexText);
//	std::cout << "摘要字符： " << encryptText << std::endl;
//	std::cout << "摘要串： " << encryptHexText << std::endl;
//
//	// sha256
//	std::cout << "=== sha256哈希 ===" << std::endl;
//	sha256(srcText, encryptText, encryptHexText);
//	std::cout << "摘要字符： " << encryptText << std::endl;
//	std::cout << "摘要串： " << encryptHexText << std::endl;
//
//	// des
//	std::cout << "=== des加解密 ===" << std::endl;
//	std::string desKey = "12345";
//	encryptText = des_encrypt(srcText, desKey);
//	std::cout << "加密字符： " << std::endl;
//	std::cout << encryptText << std::endl;
//	decryptText = des_decrypt(encryptText, desKey);
//	std::cout << "解密字符： " << std::endl;
//	std::cout << decryptText << std::endl;
//
//	// rsa
//	std::cout << "=== rsa加解密 ===" << std::endl;
//	std::string key[2];
//	generateRSAKey(key);
//	std::cout << "公钥: " << std::endl;
//	std::cout << key[0] << std::endl;
//	std::cout << "私钥： " << std::endl;
//	std::cout << key[1] << std::endl;
//	encryptText = rsa_pub_encrypt(srcText, key[0]);
//	std::cout << "加密字符： " << std::endl;
//	std::cout << encryptText << std::endl;
//	decryptText = rsa_pri_decrypt(encryptText, key[1]);
//	std::cout << "解密字符： " << std::endl;
//	std::cout << decryptText << std::endl;
//	*/


#include "Command.h"

void command(WebsocketServer& server, DB_Tool& dbt, File_Manager& f_m)
{
	//还没有做输入检查，比不上自动机写出来的，但加点检查总过得去
	//一个命令台只管一个端口和线程
	bool done = false;
	m_accrod accord;
	std::string input;
	while (!done) {
		std::cout << "Enter Command: ";
		std::getline(std::cin, input);

		if (input == "quit") {
			done = true;
		}
		else if (input == "help") {
			std::cout
				<< "\nCommand List:\n"
				<< "send <id message>\n"
				<< "check: Show all the client status\n"
				<< "help: Display this help text\n"
				<< "write: Write the data buffer into the connected database\n"
				<< "load: Write the message buffer into the system file\n"
				<< "answer: Select the database to send the struct to the client\n"
				<< "close <id reason>\n"
				<< "quit: Exit the program\n"
				<< std::endl;
		}
		else if (input.substr(0, 4) == "send") {
			std::stringstream ss(input);

			std::string cmd;
			int id = 0;
			std::string message;
			//stringstream 是在里面模拟一次输入，遇到空格就会截停一次，从而实现空格制导的命令模式
			ss >> cmd;
			ss >> id;
			std::getline(ss, message);

			server.Send(id, message);
		}
		else if (input.substr(0, 5) == "close") {
			std::stringstream ss(input);

			std::string cmd;
			int id = 0;
			std::string reason;
			//stringstream 是在里面模拟一次输入，遇到空格就会截停一次，从而实现空格制导的命令模式
			ss >> cmd;
			ss >> id;
			std::getline(ss, reason);

			server.Close(id, reason);
		}
		else if (input.substr(0, 5) == "check") {
			server.Check();
		}
		else if (input.substr(0, 5) == "write") {
			dbt.buffer_to_database(server.get_msg_list());
		}
		else if (input.substr(0, 4) == "load") {
			f_m.buffer_to_file("F:\\WorkPlace\\VS\\WebSocketpp_Server_Test\\write_test.txt", server.get_recv_msg());
		}
		else if (input.substr(0, 6) == "answer") {
			auto& Q = server.get_request_list();
			while (Q.size())
			{
				PatientData_t data;
				auto info = Q.front();
				if (dbt.DB_Read(data, info.second)) {
					string message = accord.encode(data);
					server.Send(info.first, '#'+ message);
				}
				Q.pop();
				printf("The rest of the unanswer list: %zd\n", Q.size());
			}
		}
		else if (input.substr(0, 4) == "quit") {
			done = true;
		}
		else {
			std::cout << "> Unrecognized Command" << std::endl;
		}
	}
}