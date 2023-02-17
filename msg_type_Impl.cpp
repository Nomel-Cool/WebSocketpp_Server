#include"msg_type.h"

PatientData_t m_accrod::decode(string text)
{
	//由于已知text信息结构，干脆一分为二处理
	PatientData_t ret_data;
	string header = text.substr(0, M_HEADER), data = text.substr(M_HEADER, text.size() - M_HEADER);
	int header_pos = 0, header_count = 0, data_pos = 0, data_count = 0;

	//Read uid
	header_count += M_UID_HEADER_LEN;
	for (; header_pos < header_count; header_pos++)
		data_count += (data_count * 10 + (header[header_pos] - '0'));
	ret_data.uid = data.substr(data_pos, data_count);
	data_pos += data_count;	//数据头往后移动
	data_count = 0;	//清空数据长度计数

	//Read patientName
	header_count += M_PATIENT_NAME_HEADER_LEN;
	for (; header_pos < header_count; header_pos++)
		data_count += (data_count * 10 + (header[header_pos] - '0'));
	ret_data.patientName = data.substr(data_pos, data_count);
	data_pos += data_count;	//数据头往后移动
	data_count = 0;

	//Read deviceName
	header_count += M_DEVICE_NAME_HEADER_LEN;
	for (; header_pos < header_count; header_pos++)
		data_count += (data_count * 10 + (header[header_pos] - '0'));
	ret_data.deviceName = data.substr(data_pos, data_count);
	data_pos += data_count;	//数据头往后移动
	data_count = 0;

	//Read studyld
	header_count += M_STUDYLD_HEADER_LEN;
	for (; header_pos < header_count; header_pos++)
		data_count += (data_count * 10 + (header[header_pos] - '0'));
	//ret_data.studyld = atoi(text.substr(data_pos, data_count).c_str()); //atoi只能转int，可能会溢出
	//下两使用流可以转更大的数据
	istringstream is(data.substr(data_pos, data_count));
	is >> ret_data.studyld;
	is.clear();
	data_pos += data_count;	//数据头往后移动
	data_count = 0;

	//Read seriesld
	header_count += M_SERIESLD_HEADER_LEN;
	for (; header_pos < header_count; header_pos++)
		data_count += (data_count * 10 + (header[header_pos] - '0'));
	//复用一下is
	is.str(data.substr(data_pos, data_count));
	is >> ret_data.seriesld;
	data_pos += data_count;	//数据头往后移动
	data_count = 0;

	return ret_data;
}

string m_accrod::encode(PatientData_t data)
{
	string text;

	//uid check
	if (data.uid.size() > 100 || data.uid.size() < 0)
	{
		printf("Wrong Uid.");
		return "";
	}
	else
	{
		size_t size_uid = data.uid.size();
		text += (to_string(size_uid / 100 % 10) + to_string(size_uid / 10 % 10) + to_string(size_uid % 10));
	}

	//p_name check
	if (data.patientName.size() > 15 || data.patientName.size() < 0)
	{
		printf("Wrong Patient Name.");
		return "";
	}
	else
	{
		size_t size_p_name = data.patientName.size();
		text += (to_string(size_p_name / 100 % 10) + to_string(size_p_name / 10 % 10) + to_string(size_p_name % 10));
	}

	//device name check
	if (data.deviceName.size() > 15 || data.deviceName.size() < 0)
	{
		printf("Wrong Device Name.");
		return "";
	}
	else
	{
		size_t size_d_name = data.deviceName.size();
		text += (to_string(size_d_name / 100 % 10) + to_string(size_d_name / 10 % 10) + to_string(size_d_name % 10));
	}

	//studyld check
	if (data.studyld < 0)
	{
		printf("Wrong StuLd.");
		return "";
	}
	else
	{
		string stu_ld = to_string(data.studyld);
		size_t size_stu_ld = stu_ld.size();
		text += (to_string(size_stu_ld / 100 % 10) + to_string(size_stu_ld / 10 % 10) + to_string(size_stu_ld % 10));
	}

	//seriseld check
	if (data.seriesld < 0)
	{
		printf("Wrong SerLd.");
		return "";
	}
	else
	{
		string ser_ld = to_string(data.seriesld);
		size_t size_ser_ld = ser_ld.size();
		text += (to_string(size_ser_ld / 100 % 10) + to_string(size_ser_ld / 10 % 10) + to_string(size_ser_ld % 10));
	}

	//接下来协议头装好了，就可以把内容装进去了
	text += (data.uid + data.patientName + data.deviceName + to_string(data.studyld) + to_string(data.seriesld));
	return text;
}