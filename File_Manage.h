#ifndef FILE_MANAGE_H
#define FILE_MANAGE_H

#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
#include <queue>

using namespace std;

class File_Manager
{
public:
	void F_Write(const string& file_name, const string& f_msg);
	bool buffer_to_file(const string& file_name, queue<string>& recMsgList);
	string GetToday();
private:
};
#endif // !FILE_MANAGE_H
