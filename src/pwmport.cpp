#include "pwmport.h"

void PWMPort::write_file(string path, string value){
   ofstream file;
   file.open(path);
   file << value;
   file.close();
}

void PWMPort::enable_port(){
   write_file(exp_l, this->port);
   write_file(enable_l, "1");
}

void PWMPort::disable_port(){
   write_file(enable_l, "0");
}

void PWMPort::set_period(string period){
   write_file(period_l, period);
}

void PWMPort::set_duty_cycle(string cycle){
   write_file(d_c_l, cycle);
}
