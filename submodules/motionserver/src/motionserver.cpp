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
#include <stdio.h>
#include <string>

using boost::asio::ip::udp;

UDPMotionServer::UDPMotionServer( boost::asio::io_service &io_service, unsigned int port )
  : socket( io_service, udp::endpoint( udp::v4(), port ) ){}


std::string 
UDPMotionServer::CalculateCycle(std::string stval){

   short val = boost::lexical_cast<short>(stval);
   double percentage = (double)val * 0.01;
   int ret = 1000000 + (percentage * 1000000);
   return to_string(ret);
}

std::string
UDPMotionServer::MotionByPercentage(std::string command, size_t len){
   short m_size = 2;
   if (len ==6)
      m_size = 4;
         
   std::string com = command.substr(0,3);
   std::string val = command.substr(3,m_size);
     
   return com + val;  
}

std::string
UDPMotionServer::MotionByDirection(const char * command){
   short val = 0; 
   const char * s;
   std::string com; 

   if ( (s = strstr(command, "HTTP")) != NULL){
      s = s + strlen("HTTP");
      std::string tmp(s,s+3);
      s = s + 3;
      com = tmp;
      std::string direction(s,s+3);
      val = boost::lexical_cast<short>(this->ports[com]->pos);

      if (direction == "pos"){
         if (val <= 95)
            val = val + 5;
      }
      else{
         if (val >= 5)
            val = val - 5;
      }
   } 
   return com + std::to_string(val);
}

void 
UDPMotionServer::StartServer()
{
  char data[4096];
  size_t maxLength = sizeof(data);

  for(;;)
    {
      size_t len = socket.receive_from( boost::asio::buffer(data,maxLength), remoteEP );
      std::string command(data,len);
      std::string com;       
      std::cout << command << std::endl;
      if (len > 6)
         com = MotionByDirection(command.c_str());
      else
         com = MotionByPercentage(command, len);

      std::string percentage = com.substr(3,3);
      com = com.substr(0,3);

      this->ports[com]->pos = percentage;
      this->ports[com]->set_duty_cycle(CalculateCycle(percentage));
                  
      std::cout << "Received command " << command << std::endl;
    }
}

int
UDPMotionServer::SendResponse( std::string const & msg )
{
  return socket.send_to( boost::asio::buffer( msg ), remoteEP );
}

