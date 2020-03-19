#include "motionserver.h"
#include <boost/lexical_cast.hpp>
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


std::string 
UDPMotionServer::CalculatePercentage(std::string stval){

   short val = boost::lexical_cast<short>(stval);
   double percentage = (double)val * 0.01;
   int ret = 1000000 + (percentage * 1000000);
   return to_string(ret);
}

void 
UDPMotionServer::StartServer()
{
  char data[4096];
  size_t maxLength = sizeof(data);

  for(;;)
    {
      size_t len = socket.receive_from( boost::asio::buffer(data,maxLength), remoteEP );
      short m_size = 2;
      if (len ==6)
        m_size = 4;
      std::cout << "Received message of size " << len << std::endl;
      std::string command(data,len);
      
      std::string com = command.substr(0,3);
      std::string val = command.substr(3,m_size);
     
      this->ports[com]->set_duty_cycle(CalculatePercentage(val));
      
      std::cout << CalculatePercentage(val) << std::endl;
      std::cout << com << std::endl;
      std::cout << val << std::endl;
            
      std::cout << "Received command " << command << std::endl;
    }
}

int
UDPMotionServer::SendResponse( std::string const & msg )
{
  return socket.send_to( boost::asio::buffer( msg ), remoteEP );
}

