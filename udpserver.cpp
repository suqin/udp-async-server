#include "udpserver.h"
void Server::start_io(int i)
{
    if (i >= block_num_){
        i=0; 
    }
    io_socket_.async_receive_from(
        boost::asio::buffer(buff[i]), io_point_, 
        boost::bind(&Server::receive_from_camera, this,i,  
                    boost::asio::placeholders::error, 
                    boost::asio::placeholders::bytes_transferred)); 
}
void Server::receive_from_camera(int i, const boost::system::error_code& error, 
                                      std::size_t size)
{
    std::size_t send_size=0; 
    send_size = (size<block_size_)?size:block_size_; 	
    std::map <udp::endpoint, bool >::iterator iter; 
    for(iter=ip_map_.begin(); iter!=ip_map_.end(); iter++){
        server_socket_.async_send_to(boost::asio::buffer(buff[i], send_size), iter->first,
            boost::bind(&Server::send_to_every_client, this, 
                        boost::asio::placeholders::error, 
                        boost::asio::placeholders::bytes_transferred)); 
    }
    std::cout << __FUNCTION__ << std::endl; 
    start_io(i+1); 
}
   
void Server::send_to_every_client( const boost::system::error_code& error,std::size_t size)
{
    std::cout << __FUNCTION__ << std::endl; 
}
void Server::start_serve()
{
    char temp[10]; 
    udp::endpoint _endpoint; 
    std::cout << __FUNCTION__ << std::endl; 
    server_socket_.async_receive_from(
      boost::asio::buffer(temp, 10), _endpoint, 
      boost::bind(&Server::receive_from_client, this, _endpoint, 
                 boost::asio::placeholders::error, 
                 boost::asio::placeholders::bytes_transferred)); 
}
void Server::receive_from_client(udp::endpoint point, const boost::system::error_code& error, std::size_t size)
{
    std::map<udp::endpoint, bool>::iterator iter; 
    iter = ip_map_.find(point); 
    if(iter  == ip_map_.end()){
        ip_map_[point] = !flag_;  
        std::cout << __FUNCTION__  << "add a new connection"<< std::endl; 
    }else{
        if(iter->second == !flag_){
            return; 
        }else{
            iter->second=!flag_; 
            std::cout << __FUNCTION__  << "update flag"<< std::endl; 
        }
    }
    start_serve(); 
}
void Server::clean_dead_point( )
{
    std::map<udp::endpoint, bool>::iterator iter; 
    for(iter=ip_map_.begin(); iter!=ip_map_.end(); iter++){
        if(iter->second == flag_){
            ip_map_.erase(iter); 
        std::cout << __FUNCTION__  << "clean up a connection"<<  std::endl; 
        }
    }
    flag_=!flag_; 
    std::cout << __FUNCTION__  << "another 10 seconds"<<  std::endl; 
    wait(); 
}
Server::Server(boost::asio::io_service& io_ser)
        : io_service_(io_ser), 
        server_socket_(io_ser, udp::endpoint(udp::v4(), server_port_)), 
        io_socket_(io_ser, udp::endpoint(udp::v4(), io_port_)), 
		flag_(true), 
		t_(io_service_, boost::posix_time::seconds(time_wait))
{
    start_io(0); 
    start_serve();
    t_.async_wait(boost::bind(&Server::clean_dead_point, this));
}
void Server::wait()
{
    if (t_.expires_from_now(boost::posix_time::seconds(time_wait)) <= 0){
        t_.async_wait(boost::bind(&Server::clean_dead_point, this));
    }
}
