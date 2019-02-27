
#include <cstdlib>
#include <string>
#include <iostream>
#include <limits>
#include <ccrtp/rtp.h>
#include "atm_service.h"
#include "atm_communication.h"

#ifdef  CCXX_NAMESPACES
using namespace ost;
using namespace std;
#endif


int main(int argc, char *argv[])
{

	string host="0.0.0.0";
	string my_ip="127.0.0.1";
	
    if (argc > 2) { 	
	    my_ip=argv[1];
	    host=argv[2];
	    string port="10000";

	    pid_t communications_pid = fork();
	
        if(communications_pid == 0)
	    {
            //Communication process
            Thread::sleep(100);
            atm_communication *communicator = new atm_communication(my_ip,host);
            communicator->start();

	    }
	    else if (communications_pid > 0)
	    {
            //Service process
            atm_service *atm_thread = new atm_service;
            atm_thread->start();

	    }
	    else
	    {
		    std::cout << "Fork fail!" << '\n';
	    }
	    while(1);
	    return 0;
	}
	else
	{
		cout<<"No argument! ./atm <ip-adress>"<<endl;
		return 0;
	}
	
}




