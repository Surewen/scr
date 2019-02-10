#include <stdlib.h>
#include <stdio.h>
#include <iostream>

bool service_running = true;

int withdraw();
int deposit();
int draw_menu();
int init_service();

int atm_service()
{
	init_service();

	while(service_running)
	{

	}
}

int withdraw()
{

}

int deposit()
{

}

int draw_menu()
{
	system("clear");
}

int init_service()
{
	std::cout << "atm_service process created." << "\n";
}