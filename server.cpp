#include "WebSocketServer.h"
//���ݴ�����
static std::wstring string_to_wstring(const std::string& s)
{
	using default_convert = std::codecvt<wchar_t, char, std::mbstate_t>;
	static std::wstring_convert<default_convert>conv(new default_convert("CHS"));
	return conv.from_bytes(s);
}
static std::string wstring_to_string(const std::wstring& s)
{
	using default_convert = std::codecvt<wchar_t, char, std::mbstate_t>;
	static std::wstring_convert<default_convert>conv(new default_convert("CHS"));
	return conv.to_bytes(s);
}
static std::string ansi_to_utf8(const std::string& s)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.to_bytes(string_to_wstring(s));
}
static std::string utf8_to_ansi(const std::string& s)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return wstring_to_string(conv.from_bytes(s));
}

// convert UTF-8 string to wstring
static int z_pos(uint8_t x)
{
	for (int i = 0; i < 5; i++, x <<= 1) {
		if ((x & 0x80) == 0)
			return i;
	}

	return 4;
}
std::wstring utf8_to_wstring(const std::string& str)
{
	std::wstring loc;
	uint8_t mask[5] = { 0x7f, 0x3f, 0x1f, 0x0f, 0x7 };

	for (size_t i = 0; i < str.length();) {
		int byte_cnt = z_pos(str[i]);
		uint16_t sum = str[i] & mask[byte_cnt];

		for (size_t j = 1; j < byte_cnt; j++) {
			sum <<= 6;
			sum |= str[i + j] & mask[1];
		}

		i += byte_cnt ? byte_cnt : 1;
		loc.push_back(sum);
	}

	return loc;
}


WebsocketServer::WebsocketServer(unsigned short port)
{
	m_WebsocketServer.set_reuse_addr(true); // �����׽���ѡ��SO_REUSEADDR

	// Set logging settings
	m_WebsocketServer.set_access_channels(websocketpp::log::alevel::all);
	m_WebsocketServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
	m_WebsocketServer.init_asio();


	//ע������������bind��д����Ӧ������2��3�Ĳ��ʹ���Աȣ�2д�����⣬3д������

	m_WebsocketServer.set_open_handler(bind(&WebsocketServer::OnOpen, this, ::_1));
	m_WebsocketServer.set_close_handler(bind(&WebsocketServer::OnClose, this, ::_1));
	m_WebsocketServer.set_message_handler(bind(&WebsocketServer::OnMessage, this, ::_1, ::_2));
	Listen_At(port);
}

WebsocketServer::~WebsocketServer()
{
	//Stop the server directly.
	m_WebsocketServer.stop();
	m_Thread->join();
}





void WebsocketServer::OnOpen(websocketpp::connection_hdl hdl) {
	//�����߼����϶�����һ��hdl���ͼ�¼�����˵�hdl,uri,remote,status����Ϊconnecting
	printf("\n");	//�����ʽ����

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_remote = con->get_remote_endpoint();
	//ÿ�����Ӷ�������ľ����Ψһ�ģ���connection_ptr������ָ�룬���ܲ�֪��ʲôʱ��ͱ������ˣ�����ȫ��ʹ��
	//ʹ��connect_hdl����ȫ����ǰ�������Ѿ��϶��ض������ӵĴ������Ű�ȫ�����ٷ���ϣ������������get_con_from_hdl����ΪhdlҲ����Ϊ��
	//����߼���Ϊ���Ͽ����Ӿ�ɾ��hdl����������������������Ϣ
	
	//������������Ϣ��hdl��Ψһ����OnClose��֤��disconnect��ɾ������ô�Ͳ������ظ���hdl��v_linked����
	websocketpp::uri_ptr p_uri = con->get_uri();
	connection_metadata t_data(hdl, to_string(con->get_state()), p_uri, client_remote);
	v_linked.push_back(t_data);	//save a linked one
	cout << "\nHave client connected: " << client_remote << endl;
}

void WebsocketServer::OnClose(websocketpp::connection_hdl hdl) {
	printf("\n");	//�����ʽ����

	//����β�hdl�ǰ�ȫ�ģ�v_linked����Ǹ��Ѿ���Close�����ص��ˣ����ٰ�ȫ��
	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_remote = con->get_remote_endpoint();
	vector<connection_metadata>::iterator it = v_linked.begin();
	cout << "\nHave client disconnected: " << client_remote << endl;
}

void WebsocketServer::OnMessage(websocketpp::connection_hdl hdl, message_ptr msg) {
	printf("\n");	//�����ʽ����

	try {
		//msgҪ�ȷ�ǰ�洦����Ȼ�߳�һ���Ϳ���
		string recMsg = msg->get_payload();		//��ȡ������Ϣ
		//����Ӧ����һ�����ܣ�recMsg

		string strMsg = utf8_to_ansi(recMsg);	//������������
		string strReceive = "Receive: " + strMsg;
		server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
		string client_ip_port = con->get_remote_endpoint();

		//����һ���ַ��Ƿ�Ϊ'/'��˵���ͻ��˷�����������Ϣ��Ӧ����ȡ��Ӧ���ݿ���Ϣ����
		//Ӧ������һ����ֵ��<�ͻ�����v_linked�е�����,������Ϣ��uid>
		if (strMsg[0] == '/')
		{
			int pos = -1;
			vector<connection_metadata>::iterator it = v_linked.begin();
			for (; it < v_linked.end(); it++)
			{
				//Ѱ��endpoint��־һ�����Ǹ��ͻ�������v_linked�е�λ��
				if (!strcmp((*it).get_port().c_str(), client_ip_port.c_str()))
				{
					pos = it - v_linked.begin();
				}
			}
			//���û�ҵ�pos�ͻ�Ϊ-1�������ǲ����ܵģ��ܷ���Ϣ�����Ŀͻ��˿϶�����¼�ڰ���
			request_list.push(pair<int, string>(pos, recMsg.substr(1, recMsg.size() - 1)));
			cout << request_list.size() << " request(s) hanging." << endl;	//UI����������Ϣ����
		}
		
		//																													////Ĭ��Ϊechoģʽ��˭�����ͷ���ȥ˭
		//																													//m_WebsocketServer.send(hdl, strRespon, websocketpp::frame::opcode::text);

		//�����͹������ǽṹ���ʱ��Żᱣ�浽�ṹ�建����
		else if (strMsg[0] == '#') {
			m_accrod accord;
			PatientData_t data = accord.decode(strMsg.substr(1, strMsg.size() - 1));	//ʵ��text --�� struct
			//��ֹ���ݿ����ӳ�����������¶������ȴ浽������Լ��Ļ�����
			p_Data.push(data);
			cout << p_Data.size() << " data(s) unsaved." << endl;	//UI����������Ϣ����
		}

		//else //��ע�͵���һ�䣬��ֻ���������Ϣд���ļ��������޲����Ϣ�洢������Э�飩
			recMsgList.push(strReceive + " from " + client_ip_port);	//ͨѶ��Ϣ�洢
		cout << recMsgList.size() << " message(s) unloaded." << endl;	//UI����������Ϣ����
	}
	catch (websocketpp::exception const& e) {
		cout << "error: " << e.what() << endl;
	}
}

bool WebsocketServer::OnPing(websocketpp::connection_hdl hdl, std::string payload) {

	printf("\n");	//�����ʽ����

	m_WebsocketServer.get_alog().write(websocketpp::log::alevel::app, payload);

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_ip_port = con->get_remote_endpoint();

	cout << "recv ping: " << payload + " from " << client_ip_port << endl;
	try {
		m_WebsocketServer.pong(hdl, "pong");
	}
	catch (websocketpp::exception const& e) {
		cout << "error: " << e.what() << endl;
	}
	return false;
}

bool WebsocketServer::OnPong(websocketpp::connection_hdl hdl, std::string payload) {
	
	printf("\n");	//�����ʽ����

	m_WebsocketServer.get_alog().write(websocketpp::log::alevel::app, payload);

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_ip_port = con->get_remote_endpoint();

	cout << "recv pong from " << client_ip_port << ": " << payload << endl;
	return false;
}

// js�ͻ������ӷ�ʽ��var ws = new WebSocket("ws://xxxxx", ["token value"]);
bool WebsocketServer::OnValidate(websocketpp::connection_hdl hdl)
{
	printf("\n");	//�����ʽ����

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_ip_port = con->get_remote_endpoint();
	string key = "Sec-WebSocket-Protocol";
	string token = con->get_request_header(key);
	std::cout << key << ": " << token << std::endl;
	if (0) { // ����������ʱֱ�ӷ���FALSE���ͻ��˻��յ�4��������Ӳ���
		return false;
	}

	// ��֤ͨ�����ͷ��������true
	con->append_header(key, token); // �Ժ�ظ����ݶ�Ҫ�����ͷ����Ϣ
	return true;
}

//��ʼ����port�˿�
server_thread WebsocketServer::Listen_At(unsigned short port) {

	//�����ɹ������߳�

	m_WebsocketServer.listen(port);
	cout << "Listening at " << port << endl;

	//Start the server accept loop
	m_WebsocketServer.start_accept();

	//Start the ASIO io_service run loop with thread
	m_Thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&server::run, &m_WebsocketServer);
	
	//m_WebsocketServer.run();
	return m_Thread;
}

bool WebsocketServer::Send(size_t i,std::string message)
{
	//��ָ���ͻ��˷�����Ϣ
	websocketpp::lib::error_code ec;

	if (i < v_linked.size())
	{
		// ���ӷ�������
		// ����Ӧ���и����ܣ�message
		m_WebsocketServer.send(v_linked[i].get_hdl(), ansi_to_utf8(message), websocketpp::frame::opcode::text, ec);
		if (ec) {
			std::cout << "> Error sending message: " << ec.message() << std::endl;
			return false;
		}
		std::cout << "�������ݳɹ�" << std::endl;
	}

	return true;
}


bool WebsocketServer::Close(size_t i, std::string reason)
{
	websocketpp::lib::error_code ec;

	if (i < v_linked.size())
	{
		//����̨�����رգ�Ĭ��Ϊnormal
		int close_code = websocketpp::close::status::normal;
		// �ر����ӣ�������Ӽ�¼
		v_linked.erase(v_linked.begin() + i);
		m_WebsocketServer.close(v_linked[i].get_hdl(), close_code, reason, ec);
		if (ec) {
			std::cout << "> Error initiating close: " << ec.message() << std::endl;
			return false;
		}
		std::cout << "�ر�Websocket���ӳɹ�" << std::endl;
	}
	return true;
}

void WebsocketServer::Check()
{
	vector<connection_metadata>::iterator it = v_linked.begin();
	for (it; it < v_linked.end(); it++)
	{
		//�����û��Լ��Ͽ�ʱ��ֻ��ص�OnClose�������ÿ���̨��Close���Ӷ�û��ɾ����Ӧ���
		//�����check��ʱ��Ӧ���ȼ��þ���Ƿ���Ч�������Ч����ɾ��
		if ((*it).get_hdl().expired())
		{
			v_linked.erase(it);
		}
		else
		{
			auto con = m_WebsocketServer.get_con_from_hdl((*it).get_hdl());
			string current_state;
			switch (con->get_state())
			{
			case 0:current_state = "Connecting"; break;
			case 1:current_state = "Open"; break;
			case 2:current_state = "Closing"; break;
			case 3:current_state = "Closed"; break;
			default:
				break;
			}
			cout << (*it).get_port() << "____" << current_state << "____" << (*it).get_uri() << endl;
		}
	}
}

queue<PatientData_t>& WebsocketServer::get_msg_list()
{
	return p_Data;
}

queue<string>& WebsocketServer::get_recv_msg()
{
	return recMsgList;
}

queue<pair<int, string> >& WebsocketServer::get_request_list()
{
	return request_list;
}

//����������̨
//void WebsocketServer::m_command()
//{
//	//��û���������飬�Ȳ����Զ���д�����ģ����ӵ����ܹ���ȥ
//	//һ������ֻ̨��һ���˿ں��߳�
//	bool done = false;
//	std::string input;
//	while (!done) {
//		std::cout << "Enter Command: ";
//		std::getline(std::cin, input);
//
//		if (input == "quit") {
//			done = true;
//		}
//		else if (input == "help") {
//			std::cout
//				<< "\nCommand List:\n"
//				<< "send <id message>\n"
//				<< "check: Show all the client status\n"
//				<< "help: Display this help text\n"
//				<< "write: Write the buffer into the connected database\n"
//				<< "close <id reason>\n"
//				<< "quit: Exit the program\n"
//				<< std::endl;
//		}
//		else if (input.substr(0, 4) == "send") {
//			std::stringstream ss(input);
//
//			std::string cmd;
//			int id = 0;
//			std::string message;
//			//stringstream ��������ģ��һ�����룬�����ո�ͻ��ͣһ�Σ��Ӷ�ʵ�ֿո��Ƶ�������ģʽ
//			ss >> cmd;
//			ss >> id;
//			std::getline(ss, message);
//
//			Send(id,message);
//		}
//		else if (input.substr(0, 5) == "close") {
//			std::stringstream ss(input);
//
//			std::string cmd;
//			int id = 0;
//			std::string reason;
//			//stringstream ��������ģ��һ�����룬�����ո�ͻ��ͣһ�Σ��Ӷ�ʵ�ֿո��Ƶ�������ģʽ
//			ss >> cmd;
//			ss >> id;
//			std::getline(ss, reason);
//
//			Close(id, reason);
//		}
//		else if (input.substr(0, 5) == "check") {
//			Check();
//		}
//		else if (input.substr(0, 5) == "write") {
//			
//		}
//		else if (input.substr(0, 4) == "quit") {
//			done = true;
//		}
//		else {
//			std::cout << "> Unrecognized Command" << std::endl;
//		}
//	}
//}


