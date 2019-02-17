#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include <atm_communication.h>
#include <atm_service.h>

pid_t communications_pid = fork();

int main(int argc, char const *argv[])
{
	if(communications_pid == 0)
	{
		//Communication process
		atm_communication();
		return 0;
	}
	else if (communications_pid > 0)
	{
		//Service process
		atm_service();
		return 0;
	}
	else
	{
		std::cout << "Fork fail!" << '\n';
	}
	return 0;
}