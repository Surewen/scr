
#include <cstdlib>
#include <sstream>
#include <ccrtp/rtp.h>

#ifdef  CCXX_NAMESPACES
using namespace ost;
using namespace std;
#endif
class Sender: public RTPSession, public TimerPort
{
public:
    Sender(const unsigned char* data, const InetHostAddress& ia,
    tpport_t port, uint32 tstamp, uint16 count) :
    RTPSession(InetHostAddress("0.0.0.0")), packetsPerSecond(10)
    {
        uint32 timestamp = tstamp? tstamp : 0;
        defaultApplication().setSDESItem(SDESItemTypeTOOL, "rtpsend demo app.");
        setSchedulingTimeout(10000);
        setExpireTimeout(1000000);

        if ( !addDestination(ia,port) ) {
            cerr << "Could not connect" << endl;
            exit();
        }

        setPayloadFormat(StaticPayloadFormat(sptPCMU));
        startRunning();

        uint16 tstampInc = getCurrentRTPClockRate()/packetsPerSecond;
        uint32 period = 1000/packetsPerSecond;
        TimerPort::setTimer(period);
        for ( int i = 0; i < count ; i++ ) {
            putData(timestamp + i*tstampInc,
                data,strlen((char *)data) + 1);
            Thread::sleep(TimerPort::getTimer());
            TimerPort::incTimer(period);
        }
    }

private:
    const uint16 packetsPerSecond;
};

void send(char message[],string adress){
	//ATM ADRESS-------
	char port[]="11000";
	char timestamp[]="10";
	char count[]="1";
	char host[]="0.0.0.0";
	char host1[]="0.0.0.0";
	char host2[]="00.0.0.0";
	char host3[]="000.0.0.0";
	char host4[]="000.00.0.0";
	
	char host5[]="000.000.0.0";
	char host6[]="000.000.00.0";
	char host7[]="000.000.000.0";
	char host8[]="000.000.000.00";
	char host9[]="000.000.000.000";

	int size=adress.size();
	if(size==7){
		strcpy(host1, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host1),
		atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==8){
		strcpy(host2, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host2),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==9){
		strcpy(host3, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host3),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==10){
		strcpy(host4, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host4),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==11){
		strcpy(host5, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host5),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==12){
		strcpy(host6, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host6),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==13){
		strcpy(host7, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host7),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==14){
		strcpy(host8, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host8),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	if(size==15){
		strcpy(host9, adress.c_str());
		Sender sender((unsigned char *)message, InetHostAddress(host9),
    atoi(port), atoi(timestamp), atoi(count));
	}else
	{
		Sender sender((unsigned char *)message, InetHostAddress(host),
    atoi(port), atoi(timestamp), atoi(count));
	}
	
	
	//------------------
	
	
	
    
}

class Listener: RTPSession
{
public:

	ifstream accounts;
	ifstream log;
	
	string card_number = "";
	string pin;
	string balance;
	bool account_exists = false;

    Listener(InetMcastAddress& ima, tpport_t port) :
    RTPSession(ima,port) {}

    Listener(InetHostAddress& ia, tpport_t port) :
    RTPSession(ia,port) {}

	void add_log(string log_line)
	{
	log.open("log.txt");
	string line;
	string full_file;
	while(log.good())
		{
			getline(log,line,'\n');
			full_file.append(line);
			full_file.append("\n");
		}
	full_file.append(log_line);
	
	log.close();
	ofstream out_file("log.txt");
	out_file << full_file;
	
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

	bool substract_balance(string account_number, int amount)
	{
		bool flag=false;
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
				if(stoi(balance)<amount){
					flag=false;
				}else{
				balance = to_string(stoi(balance) - amount);
					flag=true;
				}
			}
			full_file.append(balance);
			full_file.append("\n");
		}
		accounts.close();
		ofstream out_file("accounts.csv");
		out_file << full_file;
		return flag;
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

	bool transfer(string sender, string receiver, int amount)
	{
		if(substract_balance(sender, amount)){
		add_balance(receiver, amount);
		return true;
		}else{
		return false;
		}
	}
	
    void listen()
    {
		char message[50];
        defaultApplication().setSDESItem(SDESItemTypeTOOL,"rtplisten demo app.");
        setExpireTimeout(1000000);
        setPayloadFormat(StaticPayloadFormat(sptPCMU));
        startRunning();
        stringstream ad_buffer;
        string adresstring;
        
        
        for (;;) {
            const AppDataUnit* adu;
            while ( (adu = getData(getFirstTimestamp())) ) {
				

					
					ad_buffer.clear();
					
                     ad_buffer.str(string());
                     ad_buffer<< adu->getSource().getNetworkAddress();
                     adresstring = ad_buffer.str();


                    
                     stringstream loglinestream;
                     
                     loglinestream<< "Request from: "
                     << dec <<
					 adu->getSource().getNetworkAddress()
                     << ":"
                     << adu->getSource().getDataTransportPort()<<" ";
                     string logline=loglinestream.str();


                     stringstream buffer;
				buffer<< hex << adu->getData()<<endl;
				string temp = buffer.str();
				//mozna podzielic tutaj stringa na skladowe i do tablic char powsadzac odpowiednie pola lub pozamieniac na liczby nawet
				
				size_t pos = 0;
				string delimiter = ":";
				string command_string = temp.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				temp.erase(0, temp.find(delimiter) + delimiter.length());
				
				string account_number;
				string amount;
				string account_number2;

				char command_buf[1];  
				strcpy(command_buf, command_string.c_str());  
				//Po odebraniu danych nalezy je zdekodowac i wykonac odpowiednia akcje:
				if(command_buf[0]=='1'){
					
					//numer konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					
					
                     loglinestream<<"PIN request for account: "+account_number+". Sending PIN: "+get_pin(account_number)<<endl;
                     logline=loglinestream.str();
                      
                     
					//cout<<"PIN request for account: "+account_number+". Sending PIN: "+get_pin(account_number)<<endl;
					
					//Odeslanie PIN o danym numerze konta
					string data = "2:";
					data=data+get_pin(account_number)+string(":");
					strcpy(message, data.c_str());
					//getpin
					
					//-----------------------------------

				}else 
				if(command_buf[0]=='3'){
					
					//numer konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					
					loglinestream<<"Request balance for account: "+account_number+" Sending current balance: "+get_balance(account_number)<<endl;
                     logline=loglinestream.str();
                    
                     
					temp.erase(0, pos + delimiter.length());
					//Odeslanie salda o danym numerze konta
					string data = "4:";
					data=data+account_number+string(":")+get_balance(account_number)+string(":");
					strcpy(message, data.c_str());
					//-----------------------------------
					
				}else 
				if(command_buf[0]=='5'){
					
					//numer konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					
					//kwota
					pos = temp.find(delimiter);
					amount = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					
					loglinestream<<"Withdrawal request for account: "+account_number+" Amount: "+amount<<endl;
                     logline=loglinestream.str();
                   
                    
					//Odeslanie potwierdzenia operacji
					if(substract_balance(account_number,stoi(amount))){
					loglinestream<<"Operation success. Sending approval."<<endl;
                    logline=loglinestream.str();
                    cout<<logline<<endl;
                    
		
					string data = "7:";
					strcpy(message, data.c_str());
					}else{
				
					loglinestream<<"Operation failed. Not enough funds."<<endl;
                    logline=loglinestream.str();
                    cout<<logline<<endl;
                    
					string data = "8:";
					strcpy(message, data.c_str());
					}
					
					
					//-----------------------------------
					
					
				}else 
				if(command_buf[0]=='6'){
					
					//numer konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					
					//kwota
					pos = temp.find(delimiter);
					amount = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					
					loglinestream<<"Deposit request for account: "+account_number+" Amount: "+amount<<endl;
                    logline=loglinestream.str();
                  
					//Odeslanie potwierdzenia operacji
					add_balance(account_number,stoi(amount));
					
					loglinestream<<"Operation success. Sending approval."<<endl;
                    logline=loglinestream.str();
  
					string data = "7:";
					strcpy(message, data.c_str());
					//-----------------------------------
					
					
				}else 
				if(command_buf[0]=='9'){

					//numer konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//numer konta2
					pos = temp.find(delimiter);
					account_number2 = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//kwota
					pos = temp.find(delimiter);
					amount = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					
					
					loglinestream<<"Transfer request for account: "+account_number+" to: "+account_number2+" Amount: "+amount<<endl;
                    logline=loglinestream.str();
 
					
					if(transfer(account_number,account_number2,stoi(amount))){
						
					loglinestream<<"Operation success. Sending approval."<<endl;
                    logline=loglinestream.str();

						string data = "7:";
						strcpy(message, data.c_str());
					}else{
					
					loglinestream<<"Operation failed. Not enough funds."<<endl;
                    logline=loglinestream.str();

						string data = "8:";
						strcpy(message, data.c_str());
					}
					//Odeslanie potwierdzenia operacji
					
					//-----------------------------------
					
					
				}
				
				 cout<<logline;
                 add_log(logline);
                 
                 send(message,adresstring);    

                 
                delete adu;
            }
            Thread::sleep(7);
        }
    }
    
    

};
void startListening(){
	
	InetMcastAddress ima;
    try {
        ima = InetMcastAddress("127.0.0.1");
    } catch (...) { }

    Listener *foo;
    tpport_t port = atoi("10000");
    if ( ima.isInetAddress() ) {
        foo = new Listener(ima,port);
    } else {
        InetHostAddress ia("127.0.0.1");
        foo = new Listener(ia,atoi("10000"));
    }
    foo->listen();
    delete foo;
	
}
int main(int argc, char *argv[]){
    startListening();
    
    return 0;
}

	

/** EMACS **
 * Local variables:
 * mode: c++
 * c-basic-offset: 4
 * End:
 */
