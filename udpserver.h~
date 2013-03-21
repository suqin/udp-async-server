#include <iostream>
#include <map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::asio::ip::udp; 
class Server
{
    private:
    boost::asio::io_service& io_service_; 
    udp::socket server_socket_; 
    udp::socket io_socket_; 
	udp::endpoint io_point_; 
	std::map <udp::endpoint, bool> ip_map_;
	bool flag_; 
    int count_; 
    enum { server_port_ = 7891, io_port_ = 7892 , block_size_ = 65535, block_num_ = 20, time_wait = 10};
	char buff[block_num_][block_size_]; 
	boost::asio::deadline_timer t_; 
	public:
	void wait(); 
	void clean_dead_point( ); 
    void receive_from_camera(int i, const boost::system::error_code& error, std::size_t size); 
    void send_to_every_client(const boost::system::error_code& error, std::size_t size);
	void receive_from_client(udp::endpoint point, const boost::system::error_code& error, std::size_t size); 
    void start_io(int count_); 
    void start_serve(); 
    Server(boost::asio::io_service& io_ser); 
}; 

