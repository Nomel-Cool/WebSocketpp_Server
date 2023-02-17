#ifndef MSG_TYPE_H
#define MSG_TYPE_H
#define M_HEADER 15
#define M_UID_HEADER_LEN 3
#define M_PATIENT_NAME_HEADER_LEN 3
#define M_DEVICE_NAME_HEADER_LEN 3
#define M_STUDYLD_HEADER_LEN 3
#define M_SERIESLD_HEADER_LEN 3
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;
typedef struct PatientData_t
{
	std::string uid;
	std::string patientName;
	std::string deviceName;
	long studyld;
	long seriesld;
}PatientData_t;

class m_accrod
{
public:

	//用于接受一个定制好的协议text，用decode解读成一个PatientData_t
	PatientData_t decode(string text);

	//用于接受一个PatientData_t，用encode封装成一个text用于发送
	string encode(PatientData_t data);

private:
};

#endif // !MSG_TYPE_H
