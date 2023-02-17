#include "WebSocketServer.h"
//数据处理工具
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
	m_WebsocketServer.set_reuse_addr(true); // 设置套接字选项SO_REUSEADDR

	// Set logging settings
	m_WebsocketServer.set_access_channels(websocketpp::log::alevel::all);
	m_WebsocketServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
	m_WebsocketServer.init_asio();


	//注意在类里面用bind的写法对应，看看2和3的博客代码对比，2写在类外，3写在类内

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
	//任务逻辑：肯定来了一个hdl，就记录来的人的hdl,uri,remote,status更新为connecting
	printf("\n");	//输出格式调整

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_remote = con->get_remote_endpoint();
	//每次链接都会产生的句柄是唯一的，但connection_ptr是智能指针，可能不知道什么时候就被销毁了，不安全不使用
	//使用connect_hdl更安全（但前提是在已经肯定必定有链接的代码块里才安全），官方不希望多次在外调用get_con_from_hdl，因为hdl也可能为空
	//因此逻辑改为，断开链接就删除hdl，重新连接再重新载入信息
	
	//创建新链接信息，hdl的唯一性与OnClose保证，disconnect后删除，那么就不会有重复的hdl在v_linked里了
	websocketpp::uri_ptr p_uri = con->get_uri();
	connection_metadata t_data(hdl, to_string(con->get_state()), p_uri, client_remote);
	v_linked.push_back(t_data);	//save a linked one
	cout << "\nHave client connected: " << client_remote << endl;
}

void WebsocketServer::OnClose(websocketpp::connection_hdl hdl) {
	printf("\n");	//输出格式调整

	//这个形参hdl是安全的，v_linked里的那个已经被Close函数关掉了，不再安全了
	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_remote = con->get_remote_endpoint();
	vector<connection_metadata>::iterator it = v_linked.begin();
	cout << "\nHave client disconnected: " << client_remote << endl;
}

void WebsocketServer::OnMessage(websocketpp::connection_hdl hdl, message_ptr msg) {
	printf("\n");	//输出格式调整

	try {
		//msg要先放前面处理，不然线程一过就空了
		string recMsg = msg->get_payload();		//读取负载信息
		//这里应该有一个解密，recMsg

		string strMsg = utf8_to_ansi(recMsg);	//处理中文乱码
		string strReceive = "Receive: " + strMsg;
		server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
		string client_ip_port = con->get_remote_endpoint();

		//若第一个字符是否为'/'，说明客户端发送了请求消息，应当读取对应数据库信息返回
		//应当创建一个键值对<客户端在v_linked中的索引,请求消息的uid>
		if (strMsg[0] == '/')
		{
			int pos = -1;
			vector<connection_metadata>::iterator it = v_linked.begin();
			for (; it < v_linked.end(); it++)
			{
				//寻找endpoint标志一样的那个客户端所在v_linked中的位置
				if (!strcmp((*it).get_port().c_str(), client_ip_port.c_str()))
				{
					pos = it - v_linked.begin();
				}
			}
			//如果没找到pos就会为-1，但这是不可能的，能发消息过来的客户端肯定被记录在案了
			request_list.push(pair<int, string>(pos, recMsg.substr(1, recMsg.size() - 1)));
			cout << request_list.size() << " request(s) hanging." << endl;	//UI处进行来消息提醒
		}
		
		//																													////默认为echo模式，谁发来就发回去谁
		//																													//m_WebsocketServer.send(hdl, strRespon, websocketpp::frame::opcode::text);

		//当发送过来的是结构体的时候才会保存到结构体缓存器
		else if (strMsg[0] == '#') {
			m_accrod accord;
			PatientData_t data = accord.decode(strMsg.substr(1, strMsg.size() - 1));	//实现text --》 struct
			//防止数据库链接出现问题而导致丢包，先存到服务端自己的缓存里
			p_Data.push(data);
			cout << p_Data.size() << " data(s) unsaved." << endl;	//UI处进行来消息提醒
		}

		//else //若注释掉这一句，则只会把聊天消息写入文件，否则无差别消息存储（包括协议）
			recMsgList.push(strReceive + " from " + client_ip_port);	//通讯信息存储
		cout << recMsgList.size() << " message(s) unloaded." << endl;	//UI处进行来消息提醒
	}
	catch (websocketpp::exception const& e) {
		cout << "error: " << e.what() << endl;
	}
}

bool WebsocketServer::OnPing(websocketpp::connection_hdl hdl, std::string payload) {

	printf("\n");	//输出格式调整

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
	
	printf("\n");	//输出格式调整

	m_WebsocketServer.get_alog().write(websocketpp::log::alevel::app, payload);

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_ip_port = con->get_remote_endpoint();

	cout << "recv pong from " << client_ip_port << ": " << payload << endl;
	return false;
}

// js客户端连接方式：var ws = new WebSocket("ws://xxxxx", ["token value"]);
bool WebsocketServer::OnValidate(websocketpp::connection_hdl hdl)
{
	printf("\n");	//输出格式调整

	server::connection_ptr con = m_WebsocketServer.get_con_from_hdl(hdl);
	string client_ip_port = con->get_remote_endpoint();
	string key = "Sec-WebSocket-Protocol";
	string token = con->get_request_header(key);
	std::cout << key << ": " << token << std::endl;
	if (0) { // 不满足条件时直接返回FALSE，客户端会收到4类错误连接不上
		return false;
	}

	// 验证通过添加头部并返回true
	con->append_header(key, token); // 以后回复数据都要加这个头部信息
	return true;
}

//开始监听port端口
server_thread WebsocketServer::Listen_At(unsigned short port) {

	//监听成功返回线程

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
	//对指定客户端发送信息
	websocketpp::lib::error_code ec;

	if (i < v_linked.size())
	{
		// 连接发送数据
		// 这里应该有个加密，message
		m_WebsocketServer.send(v_linked[i].get_hdl(), ansi_to_utf8(message), websocketpp::frame::opcode::text, ec);
		if (ec) {
			std::cout << "> Error sending message: " << ec.message() << std::endl;
			return false;
		}
		std::cout << "发送数据成功" << std::endl;
	}

	return true;
}


bool WebsocketServer::Close(size_t i, std::string reason)
{
	websocketpp::lib::error_code ec;

	if (i < v_linked.size())
	{
		//控制台主动关闭，默认为normal
		int close_code = websocketpp::close::status::normal;
		// 关闭连接，清除链接记录
		v_linked.erase(v_linked.begin() + i);
		m_WebsocketServer.close(v_linked[i].get_hdl(), close_code, reason, ec);
		if (ec) {
			std::cout << "> Error initiating close: " << ec.message() << std::endl;
			return false;
		}
		std::cout << "关闭Websocket连接成功" << std::endl;
	}
	return true;
}

void WebsocketServer::Check()
{
	vector<connection_metadata>::iterator it = v_linked.begin();
	for (it; it < v_linked.end(); it++)
	{
		//由于用户自己断开时，只会回调OnClose，不调用控制台的Close，从而没有删掉对应句柄
		//因此在check的时候，应当先检查该句柄是否有效，如果无效就先删除
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

//服务器命令台
//void WebsocketServer::m_command()
//{
//	//还没有做输入检查，比不上自动机写出来的，但加点检查总过得去
//	//一个命令台只管一个端口和线程
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
//			//stringstream 是在里面模拟一次输入，遇到空格就会截停一次，从而实现空格制导的命令模式
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
//			//stringstream 是在里面模拟一次输入，遇到空格就会截停一次，从而实现空格制导的命令模式
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


