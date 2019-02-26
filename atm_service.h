
#include <cstdlib>
#include <string>
#include <iostream>
#include <limits>
#include <ccrtp/rtp.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef  CCXX_NAMESPACES
using namespace ost;
using namespace std;
#endif

class atm_service: public Thread
{
private:
public:
	atm_service(){
	//spowolnienie aby uruchomil sie po watku do komunikacji
	Thread::sleep(30);
	}
	~atm_service(){
	cout<<"ATM byebye"<<endl;
	}
	
	bool service_running = true;
	string card_number;
	string pin;
	string amount;
	int balance;
	
	void run(void){  
		  
	init_service();
	

	int current_time = 0;
	int previous_time = 0;
	int delta_time = 0;

	while(service_running)
	{
		if(delta_time > 5)
		{
			previous_time = time(0);
			abc:
			system("clear");
		
			login();
			
			if(pin_check(pin))
			{

				draw_menu();
				char key_pressed;
				cin >> key_pressed;
				switch(key_pressed)
				{
					case '1': // show balance
					system("clear");
					your_balance();
					break;
					case '2': // withdraw cash
					system("clear");
					withdrawing();
					break;
					case '3': // deposit cash
					system("clear");
					depositing();
					break;
					case '4': //transfer
					system("clear");
					transfer();
					case '5': // quit
					system("clear");
					goto abc;
					break;
					default:
					break;
				}
			}
			else
			{
				std::cout << "Wrong PIN, try again"<<endl;
			}
		
			//END OF LOGIC
			//------------
		}
		current_time = time(0);
		delta_time = current_time - previous_time;

	}
		
	}
	int draw_menu()
	{
		system("clear");
		std::cout <<
		"Main menu:\n" <<
		"1. Show balance\n" <<
		"2. Withdraw cash\n" <<
		"3. Deposit cash\n" <<
		"4. Transfer cash\n" <<
		"5. Quit\n";
	return 0;
	}

	int init_service()
	{
		cout<<"ATM hello"<<endl;
	return 0;
	}

	bool pin_check(string pin)
	{
		string user_input_pin;
		cout << "Please Enter Your 4 Digit PIN\n> ";
		cin >> user_input_pin;
		
		
		//Utworzenie zadania do wyslania do banku
		string temp = "";
		temp="1:"+card_number+string(":");
		char buf[30];  
		strcpy(buf, temp.c_str());  
				  
		//KOMUNIKACJA Z PROCESEM KOMUNIKACJ
		//WYSLANIE PROSBY O PIN
		int fd; 
		char const * myfifo = "./myfifo"; 
		mkfifo(myfifo, 0666); 
        fd = open(myfifo, O_WRONLY);
        write(fd, buf, strlen(buf)+1); 
        close(fd); 
        
		
		//ZWROTNY KOMUNIKAT odczytany z PIPE'a od procesu komunikacji
		int fd2; 
		char const * myfifo2 = "./myfifo2"; 
		char buf2[30];
		mkfifo(myfifo2, 0666); 
		fd2 = open(myfifo2,O_RDONLY); 
		read(fd2, buf2, 30); 
		close(fd2); 
		
		
		
		string message(buf2); //cala wiadomosc od banku
		
				//wybranie pierwszej skladowej (numer rozkazu)
				size_t pos = 0;
				string delimiter = ":";
				string command_string = message.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				message.erase(0, message.find(delimiter) + delimiter.length());
				
				char command_buf[1];//Zmienna przechowujaca numer rozkazu
				strcpy(command_buf, command_string.c_str()); 
				
				//Sprawdzenie czy to wiadomosc przekazujaca PIN uzytkownika
				if(command_buf[0]=='2'){
					//Wybranie PIN'u z wiadomosci
					size_t pos = 0;
					pos = message.find(delimiter);
					pin = message.substr(0, pos);
					message.erase(0, pos + delimiter.length());

				}
		
		//Porownanie PINu
		if(user_input_pin.compare(pin) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void your_balance(){

		//KOMUNIKACJA Z PROCESEM KOMUNIKACJ
		
		//Utworzenie zadania do wyslania do banku
		string temp = "";
		temp="3:"+card_number+string(":");
		char buf[30];  
		strcpy(buf, temp.c_str());  
		
		//WYSLANIE wiadomosci do banku przez PIPE'a do procesu komunikacji
		int fd; 
		char const * myfifo = "./myfifo"; 
		mkfifo(myfifo, 0666); 
        fd = open(myfifo, O_WRONLY); 
        write(fd, buf, strlen(buf)+1); 
        close(fd); 
        Thread::sleep(50);
        
        //ZWROTNY KOMUNIKAT odczytany z PIPE'a od procesu komunikacji
        int fd2; 
		char const * myfifo2 = "./myfifo2"; 
		char buf2[30];
		mkfifo(myfifo2, 0666); 
		fd2 = open(myfifo2,O_RDONLY); 
		read(fd2, buf2, 30); 
		close(fd2); 
		string message(buf2); //cala wiadomosc od banku
	
				//wybranie pierwszej skladowej (numer rozkazu)
				size_t pos = 0;
				string delimiter = ":";
				string command_string = message.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				message.erase(0, message.find(delimiter) + delimiter.length());
				char command_buf[1];//Zmienna przechowujaca numer rozkazu
				strcpy(command_buf, command_string.c_str()); 
				
				if(command_buf[0]=='4'){
					//wybranie drugiej skladowej rozkazu nr 4 : saldo
					size_t pos = 0;
					pos = message.find(delimiter);
					string account_num = message.substr(0, pos);
					message.erase(0, pos + delimiter.length());
					pos = message.find(delimiter);
					amount = message.substr(0, pos);
					message.erase(0, pos + delimiter.length());
				}
		
		cout<<"Your balance: "+amount<<endl<<endl;
		cout<<"Press enter to continue"<<endl;
		cin.ignore();
		cin.get();

	}
	void withdrawing(){
		cout<<"How much do you want to withdraw?"<<endl<<endl;
		int amount=0;
		cin >> amount;
		cout<<"Are you sure? (y/n)"<<endl;
		char key_pressed;
		cin >> key_pressed;
		if(key_pressed=='y'){
			cout<<"Withdrawing: ";
			cout<<amount<<endl;
			
			//Utworzenie zadania do wyslania do banku
			string temp = "";
			temp="5:"+card_number+":"+to_string(amount)+string(":");
			char buf[30];  
			strcpy(buf, temp.c_str());  
			
			//KOMUNIKACJA Z PROCESEM KOMUNIKACJ
			
			//WYSLANIE wiadomosci do banku przez PIPE'a do procesu komunikacji  
			int fd; 
			char const * myfifo = "./myfifo"; 
			mkfifo(myfifo, 0666); 
			fd = open(myfifo, O_WRONLY); 
			write(fd, buf, strlen(buf)+1); 
			close(fd); 
			
			//ZWROTNY KOMUNIKAT odczytany z PIPE'a od procesu komunikacji
			int fd2; 
			char const * myfifo2 = "./myfifo2"; 
			char buf2[30];
			mkfifo(myfifo2, 0666); 
			fd2 = open(myfifo2,O_RDONLY); 
			read(fd2, buf2, 30); 
			close(fd2); 
			string message(buf2);//cala wiadomosc od banku
				
				//wybranie pierwszej skladowej (numer rozkazu)
				size_t pos = 0;
				string delimiter = ":";
				string command_string = message.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				message.erase(0, message.find(delimiter) + delimiter.length());
				char command_buf[1];//Zmienna przechowujaca numer rozkazu
				strcpy(command_buf, command_string.c_str()); 
				
				if(command_buf[0]=='7'){//Numer rozkazu 7 - operacja udana
				cout<<"Operation succesful!"<<endl;
				}else 
				if(command_buf[0]=='8'){//Numer rozkazu 8 - operacja nieudana
				cout<<"Operation failed!"<<endl;		
				}
				
		
			//---------------
			
			}else if(key_pressed=='n'){
			cout<<"Aborting operation!"<<endl;
			}
		cout<<"Press enter to continue"<<endl;
		cin.ignore();
		cin.get();
	}
	void depositing(){
		cout<<"How much do you want to deposit"<<endl<<endl;
		int amount;
		cin >> amount;
		cout<<"Are you sure? (y/n)"<<endl;
		char key_pressed;
		cin >> key_pressed;
		if(key_pressed=='y'){
			cout<<"Depositing: ";
			cout<<amount<<endl;
			
			//Utworzenie zadania do wyslania do banku
			string temp = "";
			temp="6:"+card_number+":"+to_string(amount)+string(":");
			char buf[30];  
			strcpy(buf, temp.c_str());  
			
			//KOMUNIKACJA Z PROCESEM KOMUNIKACJ
			
			//WYSLANIE wiadomosci do banku przez PIPE'a do procesu komunikacji	  
			int fd; 
			char const * myfifo = "./myfifo"; 
			mkfifo(myfifo, 0666); 
			fd = open(myfifo, O_WRONLY); 
			write(fd, buf, strlen(buf)+1); 
			close(fd); 
			
			//ZWROTNY KOMUNIKAT odczytany z PIPE'a od procesu komunikacji
			int fd2; 
			char const * myfifo2 = "./myfifo2"; 
			char buf2[30];
			mkfifo(myfifo2, 0666); 
			fd2 = open(myfifo2,O_RDONLY); 
			read(fd2, buf2, 30); 
			close(fd2); 
			string message(buf2);//cala wiadomosc od banku
			
				//wybranie pierwszej skladowej (numer rozkazu)
				size_t pos = 0;
				string delimiter = ":";
				string command_string = message.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				message.erase(0, message.find(delimiter) + delimiter.length());
				char command_buf[1];//Zmienna przechowujaca numer rozkazu
				strcpy(command_buf, command_string.c_str()); 
				
				if(command_buf[0]=='7'){//Numer rozkazu 7 - operacja udana
				cout<<"Operation succesful!"<<endl;
				}else 
				if(command_buf[0]=='8'){//Numer rozkazu 8 - operacja nieudana
				cout<<"Operation failed!"<<endl;		
				}
			
			//---------------
			}else if(key_pressed=='n'){
			cout<<"Aborting operation!"<<endl;
			}
		cout<<"Press enter to continue"<<endl;
		cin.ignore();
		cin.get();
	}
	void transfer(){
		string account_number_transfer;
		cout<<"Where do you want to transfer?"<<endl<<endl;
		cin >> account_number_transfer;
		cout<<"How much do you want to transfer?"<<endl<<endl;
		int amount;
		cin >> amount;
		cout<<"Are you sure? (y/n)"<<endl;
		char key_pressed;
		cin >> key_pressed;
		if(key_pressed=='y'){
			cout<<"Transfering: ";
			cout<<amount<<endl;
			//KOMUNIKACJA Z PROCESEM KOMUNIKACJI
			
			//Utworzenie zadania do wyslania do banku
			string temp = "";
			temp="9:"+card_number+":"+account_number_transfer+":"+to_string(amount)+string(":");
			char buf[50];  
			strcpy(buf, temp.c_str());  
			
			//WYSLANIE wiadomosci do banku przez PIPE'a do procesu komunikacji
			int fd; 
			char const * myfifo = "./myfifo"; 
			mkfifo(myfifo, 0666); 
			fd = open(myfifo, O_WRONLY); 
			write(fd, buf, strlen(buf)+1); 
			close(fd); 
			
			//ZWROTNY KOMUNIKAT odczytany z PIPE'a od procesu komunikacji
			int fd2; 
			char const * myfifo2 = "./myfifo2"; 
			char buf2[50];
			mkfifo(myfifo2, 0666); 
			fd2 = open(myfifo2,O_RDONLY); 
			read(fd2, buf2, 50); 
			close(fd2); 
			
			string message(buf2); //cala wiadomosc od banku
				
				//wybranie pierwszej skladowej (numer rozkazu)
				size_t pos = 0;
				string delimiter = ":";
				string command_string = message.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				message.erase(0, message.find(delimiter) + delimiter.length());
				char command_buf[1];//Zmienna przechowujaca numer rozkazu
				strcpy(command_buf, command_string.c_str()); 
				
				if(command_buf[0]=='7'){//Numer rozkazu 7 - operacja udana
				cout<<"Operation succesful!"<<endl;
				}else 
				if(command_buf[0]=='8'){//Numer rozkazu 8 - operacja nieudana
				cout<<"Operation failed!"<<endl;		
				}

			}else if(key_pressed=='n'){
			cout<<"Aborting operation!"<<endl;
			}
		cout<<"Press enter to continue"<<endl;
		cin.ignore();
		cin.get();
		
	}

	void login()//returns user PIN
	{
		cout << "Please Enter Your Card Number\n> ";
		cin >> card_number;
	}
};
