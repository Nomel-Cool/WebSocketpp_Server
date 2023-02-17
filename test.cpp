#include "Command.h"

int main() {
	//服务端启动. 监听端口：9002
	unsigned short net_port = 9002;
	WebsocketServer server(net_port);

	//数据库操作，与服务器信息接收业务拆分
	string host = "localhost";
	string user = "root";
	string psw = "123456";
	string database = "patient_base";
	unsigned int port = 3306;
	DB_Tool dbt(host, user, psw, database, 3306);
	dbt.DB_Connect();

	//系统文件写入操作
	File_Manager f_m;

	try {
		//创建对象时已经启动了run线程了
		//可以通过new多个服务端来监听多个端口
		//但是信息会很乱，不如直接多开个服务器有效多了
		//命令台用呼出式，不然信息会堆积
		command(server, dbt, f_m);
	}
	catch (websocketpp::exception const& e) {
		cout << "error: " << e.what() << endl;
	}
	dbt.DB_Free_Connect();
	return 0;
}