#include "motionserver.h"
#include <vector>

std::map<string, PWMPort *> InitPorts(){
   
   vector<PWMPort*> ports;
   int centre = 1500000 ;
   int period = 5000000 ;
   vector<int> portnrs{3, 5,6,1};
   vector<string> names{"yaw", "pit", "ste", "spe"};
   std::map<string, PWMPort *> mapOfPorts; 

   for (unsigned int i = 0; i < portnrs.size(); i++){
      PWMPort* pp = new PWMPort(to_string(portnrs[i]));
      pp->enable_port();
      pp->set_period(to_string(period));
      pp->set_duty_cycle(to_string(centre));
      mapOfPorts.insert(std::make_pair(names[i], pp));
   }
   return mapOfPorts;
}


int main(){

   boost::asio::io_service io_service;
 
   UDPMotionServer * vs = new UDPMotionServer( io_service, 8000 );
   vs->SetPorts(InitPorts());
   vs->StartServer();  
}
