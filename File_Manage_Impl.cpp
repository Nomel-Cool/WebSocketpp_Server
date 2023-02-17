#include "File_Manage.h"

void File_Manager::F_Write(const string& file_name, const string& f_msg)
{
	fstream f;
	f.open(file_name.c_str(), ios::out | ios::app);	//׷��ģʽ
	f << f_msg << endl;	//��������
	f.close();
}
bool File_Manager::buffer_to_file(const string& file_name, queue<string>& recMsgList)
{
	//ȱʧ���ܣ�д��ʧ�ܺ�û�е���ȡbad_write_list,Ҳû�б���ʧ��ԭ��
	//write_flagĿǰ�����õģ�����д�ɽṹ���װ������Ϣ
	//ѭ���ѻ��������
	bool write_flag = true;
	while (recMsgList.size())
	{
		F_Write(file_name, GetToday() + ':' + recMsgList.front());
		recMsgList.pop();
		printf("The rest of the recieved message: %zd\n", recMsgList.size());
	}
	return write_flag;
}
string File_Manager::GetToday()
{
	time_t now = time(NULL);
	struct tm tm_t;
	localtime_s(&tm_t, &now);
	int year = tm_t.tm_year + 1900;
	int mon = tm_t.tm_mon + 1;
	int day = tm_t.tm_mday;
	int	hour = tm_t.tm_hour;
	int min = tm_t.tm_min;
	int	sec = tm_t.tm_sec;
	string time_format = "[" + to_string(year)+ '-'
							 + to_string(mon)+ '-'
							 + to_string(day) + ' ' 
							 + to_string(hour)+ ':'
							 + to_string(min) + ':'
							 + to_string(sec)
							 + ']';
	return time_format;
}