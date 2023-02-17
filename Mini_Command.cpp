//#include "WebSocketServer.h"
//#include "Database_Tools.h"
//
//
//int main() {
//	//Server on. Listen on port 9002
//	unsigned short net_port = 9002;
//	WebsocketServer server(net_port);
//
//	//���ݿ���������������Ϣ����ҵ����
//	string host = "localhost";
//	string user = "root";
//	string psw = "123456";
//	string database = "patient_base";
//	unsigned int port = 3306;
//	DB_Tool dbt(host, user, psw, database, 3306);
//	dbt.DB_Connect();
//	try {
//		//��������ʱ�Ѿ�������run�߳���
//		//����ͨ��new������������������˿�
//		//������Ϣ����ң�����ֱ�Ӷ࿪����������Ч����
//		//����̨�ú���ʽ����Ȼ��Ϣ��ѻ�
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
////	MYSQL* mysql;	//���ݿ���
////	MYSQL_RES* res = nullptr;
////	PatientData_t data;
////	data.uid = "114514";
////	data.patientName = "�����ƶ�";//����ռ������string����һ����char
////	data.deviceName = "HuaWei";
////	data.studyld = 111;
////	data.seriesld = 777;
////	//CSDN���˲�ǳ����WTF ? mysql_init(&mysql); �����ǲ��еģ�������������Ĭ�ϸ������������ٷ���һ����Ч�������mysqlһ��ʼ����Ч�Ŀյģ���ָ�������Ǵ���һ���µĸ���
////	mysql = mysql_init(nullptr);  //����mysql�����ݿ�				
////	bool connect_flag = false;	//���ӱ�־����ʾ�Ƿ���������
////	bool save_flag = false;	//�洢��־����ʾ�Ƿ�insert���ִ�гɹ�
////	const char host[] = "localhost";
////	const char user[] = "root";
////	const char psw[] = "123456";
////	const char database[] = "Patient_Base";
////	const int port = 3306;
////	////����false������ʧ�ܣ�����true�����ӳɹ�  
////	if (!(mysql_real_connect(mysql, host, user, psw, database, port, NULL, 0)))
////		//�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
////	{
////		printf("Error connecting to database:%s\n", mysql_error(mysql));	//��û�����ӵ����ݿ⣬�ͻᶪ���ģ�Ҫ��������
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
////	if (mysql_query(mysql, query.c_str())) {        //ִ��SQL��� 
////		printf("query failed (%s)\n", mysql_error(mysql));
////		save_flag = false;
////	}
////	else
////	{
////		printf("query successed.");
////		save_flag = true;
////	}
////	mysql_close(mysql);	//�Ͽ����ݿ�����
////	return 0;
////}
//




////test
//
////MYSQL_FIELD* fd;  //�ֶ�������
////char field[32][32];  //���ֶ�����ά����
////MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����
////MYSQL_ROW column; //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾ�����е���
////std::string query; //��ѯ���
////using namespace std;
//////�������� 
////bool ConnectDatabase();
////void FreeConnect();
////int main(int argc, char** argv)
////{
////
////	MYSQL mysql; //mysql����
////	//��ʼ��mysql  
////	mysql_init(&mysql);  //����mysql�����ݿ�  
////	cout << mysql_error(&mysql) << endl;
////	const char host = "localhost";
////	const char user = "root";
////	const char *psw = "123456";
////	const char *database = "Patient_Base";
////	const unsigned int port = 3306;
////	//����false������ʧ�ܣ�����true�����ӳɹ�  
////	if (!(mysql_real_connect(&mysql, host, user, psw, database, port, NULL, 0)))
////		//�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
////	{
////		printf("Error connecting to database:%s\n", mysql_error(&mysql));
////	}
////	else
////	{
////		printf("Database Connected...\n");
////	}
////	query = "select * from p_list";
////	if (mysql_query(&mysql, query.c_str()))        //ִ��SQL���  
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
//////�������ݿ�  
////bool ConnectDatabase()
////{
////	
////}
//////�ͷ���Դ  
////void FreeConnect()
////{
//////	mysql_free_result(res);  //�ͷ�һ���������ʹ�õ��ڴ档
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
//	// ԭʼ����
//	std::string srcText = "this is an example";
//
//	std::string encryptText;
//	std::string encryptHexText;
//	std::string decryptText;
//
//	std::cout << "=== ԭʼ���� ===" << std::endl;
//	std::cout << srcText << std::endl;
//
//	// md5
//	std::cout << "=== md5��ϣ ===" << std::endl;
//	md5(srcText, encryptText, encryptHexText);
//	std::cout << "ժҪ�ַ��� " << encryptText << std::endl;
//	std::cout << "ժҪ���� " << encryptHexText << std::endl;
//
//	// sha256
//	std::cout << "=== sha256��ϣ ===" << std::endl;
//	sha256(srcText, encryptText, encryptHexText);
//	std::cout << "ժҪ�ַ��� " << encryptText << std::endl;
//	std::cout << "ժҪ���� " << encryptHexText << std::endl;
//
//	// des
//	std::cout << "=== des�ӽ��� ===" << std::endl;
//	std::string desKey = "12345";
//	encryptText = des_encrypt(srcText, desKey);
//	std::cout << "�����ַ��� " << std::endl;
//	std::cout << encryptText << std::endl;
//	decryptText = des_decrypt(encryptText, desKey);
//	std::cout << "�����ַ��� " << std::endl;
//	std::cout << decryptText << std::endl;
//
//	// rsa
//	std::cout << "=== rsa�ӽ��� ===" << std::endl;
//	std::string key[2];
//	generateRSAKey(key);
//	std::cout << "��Կ: " << std::endl;
//	std::cout << key[0] << std::endl;
//	std::cout << "˽Կ�� " << std::endl;
//	std::cout << key[1] << std::endl;
//	encryptText = rsa_pub_encrypt(srcText, key[0]);
//	std::cout << "�����ַ��� " << std::endl;
//	std::cout << encryptText << std::endl;
//	decryptText = rsa_pri_decrypt(encryptText, key[1]);
//	std::cout << "�����ַ��� " << std::endl;
//	std::cout << decryptText << std::endl;
//	*/


#include "Command.h"

void command(WebsocketServer& server, DB_Tool& dbt, File_Manager& f_m)
{
	//��û���������飬�Ȳ����Զ���д�����ģ����ӵ����ܹ���ȥ
	//һ������ֻ̨��һ���˿ں��߳�
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
			//stringstream ��������ģ��һ�����룬�����ո�ͻ��ͣһ�Σ��Ӷ�ʵ�ֿո��Ƶ�������ģʽ
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
			//stringstream ��������ģ��һ�����룬�����ո�ͻ��ͣһ�Σ��Ӷ�ʵ�ֿո��Ƶ�������ģʽ
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