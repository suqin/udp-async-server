#include <iostream>
#include <boost/asio.hpp>
#include "udpserver.h"
int main()
{
    boost::asio::io_service s; 
    Server ser(s);
    s.run(); 
    return 0; 
}
