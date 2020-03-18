#ifndef __UDP_MOTION_SERVER_HPP__
#define __UDP_MOTION_SERVER_HPP__

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <map>
#include "pwmport.h"


class UDPMotionServer
{
public:
  UDPMotionServer(boost::asio::io_service & io_service, unsigned int port);
  void StartServer();
  void SetPorts(std::map<int, PWMPort*> ports){ this->ports = ports;}
 
private:
  std::map<int, PWMPort *> ports;
  int SendResponse( std::string const & msg );

private:
  boost::asio::ip::udp::socket socket;
  boost::asio::ip::udp::endpoint remoteEP;
  boost::array<char,1> receiveBuffer;

};

#endif /* __UDP_MOTION_SERVER_HPP__ */
