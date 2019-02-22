
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
	
	int pipeStatus;
	int pipeOutput[2]; // file descriptor

	pipeStatus = pipe(pipeOutput); // create output read/write pipe ends
	if (pipeStatus < 0) {
	std::cerr << "ERROR CREATING PIPE" << std::endl;
	exit(1);
	}
	
	pid_t communications_pid = fork();
	
	if(communications_pid == 0)
	{
		//Communication process
		Thread::sleep(100);
		atm_communication *communicator = new atm_communication;
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
	
	
	
	/*
	
	char data[]="Witam";
	char host[]="127.0.0.1";
	char port[]="10000";
	sendToBank(data,host,port);
	
	while(true);
	
    return 0;
    */
}




