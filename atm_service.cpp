#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

using namespace std;
bool service_running = true;



int withdraw();
int deposit();
int draw_menu();
int init_service();
bool pin_check(int pin);

int atm_service()
{
	init_service();

	int current_time = 0;
	int previous_time = 0;
	int delta_time = 0;

	while(service_running)
	{
		if(delta_time > 500)
		{
			previous_time = time(0);
			//ALL LOGIC GOES HERE


			if(pin_check(1234))
			{
				draw_menu();
				char key_pressed;
				cin >> key_pressed;
				switch(key_pressed)
				{
					case 1: // show balance
					break;
					case 2: // withdraw cash
					break;
					case 3: // deposit cash
					break;
					case 4: // quit
					exit(0);
					break;
					default:
					break;
				}
			}
			
		}
		current_time = time(0);
		delta_time = current_time - previous_time;
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
	std::cout << 
	"Main menu:\n" <<
	"1. Show balance\n" <<
	"2. Withdraw cash\n" <<
	"3. Deposit cash\n" <<
	"4. Quit\n";
}

int init_service()
{
	std::cout << "atm_service process created." << "\n";
}

bool pin_check(int pin)
{
	int user_input_pin;                                            
    cout << "Please Enter Your 4 Digit PIN\n> ";
    cin >> user_input_pin;
    if(user_input_pin == pin)
    {
    	return true;
    }
    else
    {
    	return false;
    }
}