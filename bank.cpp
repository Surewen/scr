#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

ifstream accounts;

string card_number = "";
string pin;
string balance;
bool account_exists = false;

string get_pin(string account_number);
string get_balance(string account_number);
void substract_balance(string account_number, int amount);
void add_balance(string account_number, int amount);

int main(int argc, char const *argv[])
{	
    return 0;
}

string get_pin(string account_number)
{
	accounts.open("accounts.csv");
	while(accounts.good())
	{
		getline(accounts, card_number, ',');
		getline(accounts, pin, ',');
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
		return pin;
	}
	else
	{
		return "0";
	}
}

string get_balance(string account_number)
{
	accounts.open("accounts.csv");
	while(accounts.good())
	{
		getline(accounts, card_number, ',');
		getline(accounts, pin, ',');
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
		return balance;
	}
	else
	{
		return "0";
	}
}

void substract_balance(string account_number, int amount)
{
	string full_file;
	accounts.open("accounts.csv");
	while(accounts.good())
	{
		getline(accounts, card_number, ',');
		if(card_number == "")
			break;
		full_file.append(card_number);
		full_file.append(",");
		getline(accounts, pin, ',');
		full_file.append(pin);
		full_file.append(",");
		getline(accounts, balance, '\n');
		if(card_number == account_number)
		{
			balance = to_string(stoi(balance) - amount);
		}
		full_file.append(balance);
		full_file.append("\n");
	}
	accounts.close();
	ofstream out_file("accounts.csv");
	out_file << full_file;
}

void add_balance(string account_number, int amount)
{
	string full_file;
	accounts.open("accounts.csv");
	while(accounts.good())
	{
		getline(accounts, card_number, ',');
		if(card_number == "")
			break;
		full_file.append(card_number);
		full_file.append(",");
		getline(accounts, pin, ',');
		full_file.append(pin);
		full_file.append(",");
		getline(accounts, balance, '\n');
		if(card_number == account_number)
		{
			balance = to_string(stoi(balance) + amount);
		}
		full_file.append(balance);
		full_file.append("\n");
	}
	accounts.close();
	ofstream out_file("accounts.csv");
	out_file << full_file;
}

void transfer(string sender, string receiver, int amount)
{
	substract_balance(sender, amount);
	add_balance(receiver, amount);
}