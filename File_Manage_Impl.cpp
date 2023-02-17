#include "File_Manage.h"

void File_Manager::F_Write(const string& file_name, const string& f_msg)
{
	fstream f;
	f.open(file_name.c_str(), ios::out | ios::app);	//追加模式
	f << f_msg << endl;	//输入内容
	f.close();
}
bool File_Manager::buffer_to_file(const string& file_name, queue<string>& recMsgList)
{
	//缺失功能：写入失败后，没有导向取bad_write_list,也没有保留失败原因
	//write_flag目前摆设用的，可以写成结构体封装错误信息
	//循环把缓存区清空
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