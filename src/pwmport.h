#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "httpd.h"

using namespace std;


class PWMPort{
 
 private:
   string port;
   string base = "/sys/class/pwm/pwmchip0/";
   string exp_l = base + "export";
   string unexport = base + "unexport";
   string enable_l; 
   string period_l;
   string d_c_l;

   void write_file(string path, string value);

 public:
   string pos = "50";
   static int CommandProcessMotion(int port, char const * command, char * response, unsigned int maxResponseLength);

   static struct Command const Commands[];
   PWMPort(string port){
      this->port = port;
      string pwm_port = base + "pwm" + port + "/";
      enable_l = pwm_port + "enable"; 
      period_l = pwm_port + "period";
      d_c_l = pwm_port + "duty_cycle";
   }
  
   void enable_port(); 
   void disable_port();
   void set_period(string period);
   void set_duty_cycle(string cycle); 
};


