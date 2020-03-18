#include "motionserver.h"
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

#include <ctime>
#include <iostream>
#include <syslog.h>
#include <unistd.h>
#include <string>

using boost::asio::ip::udp;

UDPMotionServer::UDPMotionServer( boost::asio::io_service &io_service, unsigned int port )
  : socket( io_service, udp::endpoint( udp::v4(), port ) ){}

void 
UDPMotionServer::StartServer()
{
  char data[4096];
  size_t maxLength = sizeof(data);

  for(;;)
    {
      size_t len = socket.receive_from( boost::asio::buffer(data,maxLength), remoteEP );
      std::cout << "Received message of size " << len << std::endl;
      std::string command(data,len);
      std::cout << "Received command " << command << std::endl;
    }
}

int
UDPMotionServer::SendResponse( std::string const & msg )
{
  return socket.send_to( boost::asio::buffer( msg ), remoteEP );
}

