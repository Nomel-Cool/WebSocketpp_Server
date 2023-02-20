#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <thread>
#include <locale>
#include <codecvt>
#include <queue>
#include <map>

// ������TLS Server
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/uri.hpp>
#include <websocketpp/common/memory.hpp>

#include "msg_type.h"


// ����TLS Server
// #include <websocketpp/config/asio_server.hpp>
// #include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> server_thread; 

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using namespace std;

class connection_metadata {
public:
	typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;
	connection_metadata(websocketpp::connection_hdl hdl, string status, websocketpp::uri_ptr uri, string remote)
		: m_Hdl(hdl)
		, m_Status(status)
		, m_Uri(uri)
		, m_Remote(remote)
	{}

	websocketpp::connection_hdl get_hdl() const
	{
		return m_Hdl;
	}

	void set_status(std::string changed_status)
	{
		m_Status.clear();
		m_Status = changed_status;
	}

	std::string get_status() const
	{
		return m_Status;
	}

	std::string get_uri() const
	{
		return to_string(m_Uri->get_port());
	}

	std::string get_port() const
	{
		return m_Remote;
	}

private:
	websocketpp::connection_hdl m_Hdl;  // websocketpp��ʾ���ӵı��
	string m_Status;					// �����Զ�״̬
	websocketpp::uri_ptr m_Uri;			// ���ӵ�URIָ��
	string m_Remote;					// Զ����Ϣ
	string m_Error_reason;				// ����ԭ��
};

//////CLASS WebsocketServer
class WebsocketServer
{
public:
	WebsocketServer(unsigned short port);
	virtual~WebsocketServer();

public:
	void OnOpen(websocketpp::connection_hdl hdl);
	void OnClose(websocketpp::connection_hdl hdl);
	void OnMessage(websocketpp::connection_hdl hdl, message_ptr msg);
	bool OnPing(websocketpp::connection_hdl hdl, std::string payload);
	bool OnPong(websocketpp::connection_hdl hdl, std::string payload);
	bool OnValidate(websocketpp::connection_hdl hdl);
	server_thread Listen_At(unsigned short port);
	
	
	//void m_command();
	bool Send(size_t i,std::string message);
	bool Close(size_t i, std::string reason);
	void Check();
	queue<PatientData_t>& get_msg_list();
	queue<string>& get_recv_msg();
	queue<pair<int, string> >& get_request_list();

private:
	server m_WebsocketServer;
	vector<connection_metadata> v_linked; //���Ӿ���洢����
	queue<PatientData_t> p_Data;		//���ݻ���
	queue<string> rec_msg_list;				//ͨѶ��Ϣ����
	websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_Thread; // �߳�
	queue<pair<int,string> > request_list;//����Ⱥ��<v_linked[int],uid>
};

#endif // !WEBSOCKET_SERVER_H

