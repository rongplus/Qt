#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "server.h"

int main(void) {
	try {
		std::cout << "server start." << std::endl;

		//std::cout << boost::asio::ip::tcp::v4() << std::endl;

		// 建造服务对象
		boost::asio::io_service ios;
		// 具体的服务器地址与端口
		boost::asio::ip::tcp::endpoint endpotion(boost::asio::ip::tcp::v4(), 8811);
		// 构建Server实例
		Server server(ios, endpotion);
		// 启动异步调用事件处理循环
		server.run();
	}
	catch (std::exception& _e) {
		std::cout << _e.what() << std::endl;
	}
	std::cout << "server end." << std::endl;
	return 0;
}