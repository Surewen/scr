#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ifstream accounts;

string card_number = "";
string pin;
string name;
string balance;
bool account_exists = false;

string get_pin(string account_number);

int main(int argc, char const *argv[])
{	
	get_pin("1234321");
    return 0;
}

string get_pin(string account_number)
{
	accounts.open("accounts.csv");
	while(accounts.good())
	{
		getline(accounts, card_number, ',');
		getline(accounts, pin, ',');
		getline(accounts, name, ',');
		getline(accounts, balance, '\n');
		if(account_number == card_number)
		{
			account_exists = true;
			break;
		}
	}
	accounts.close();
	if(account_exists)
	{
		return card_number;
	}
	else
	{
		return "0";
	}
}