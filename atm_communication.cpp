#include <stdlib.h>
#include <stdio.h>
#include <iostream>

bool communication_running = true;

int init_communication();

int atm_communication()
{
	init_communication();

	while(communication_running)
	{

	}
}

int init_communication()
{
	std::cout << "atm_communication process created." << "\n";
}