#include "Command.h"

int main() {
	//���������. �����˿ڣ�9002
	unsigned short net_port = 9002;
	WebsocketServer server(net_port);

	//���ݿ���������������Ϣ����ҵ����
	string host = "localhost";
	string user = "root";
	string psw = "123456";
	string database = "patient_base";
	unsigned int port = 3306;
	DB_Tool dbt(host, user, psw, database, 3306);
	dbt.DB_Connect();

	//ϵͳ�ļ�д�����
	File_Manager f_m;

	try {
		//��������ʱ�Ѿ�������run�߳���
		//����ͨ��new������������������˿�
		//������Ϣ����ң�����ֱ�Ӷ࿪����������Ч����
		//����̨�ú���ʽ����Ȼ��Ϣ��ѻ�
		command(server, dbt, f_m);
	}
	catch (websocketpp::exception const& e) {
		cout << "error: " << e.what() << endl;
	}
	dbt.DB_Free_Connect();
	return 0;
}