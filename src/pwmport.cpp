#include "pwmport.h"
#include "stdio.h"
#include <string.h>
#include "udpclient.h"

void 
PWMPort::write_file(string path, string value){
   ofstream file;
   file.open(path);
   file << value;
   file.close();
}

void 
PWMPort::enable_port(){
   write_file(exp_l, this->port);
   write_file(enable_l, "1");
}

void 
PWMPort::disable_port(){
   write_file(enable_l, "0");
}

void 
PWMPort::set_period(string period){
   write_file(period_l, period);
}

void 
PWMPort::set_duty_cycle(string cycle){
   write_file(d_c_l, cycle);
}

int 
PWMPort::CommandProcessMotion(int port, char const * command, char * response, unsigned int maxResponseLength){
   boost::asio::io_service io_service;
   UDPClient client (io_service, "localhost", "8000");
  
   const char* s;
   if ((s = strstr(command, "command=processmotion")) != NULL){
      if ((s = strstr(s, "mode=")) != NULL){
         s = s + strlen("mode=");
         string msg(s, s+3);
         if ((s = strstr(s, "direction=")) != NULL){
            msg += s + strlen("direction=");
            cout << msg << endl;
            client.send("HTTP" + msg);
         } 
      }
   } 
   return 0;
}
