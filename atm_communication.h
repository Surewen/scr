#include <cstdlib>
#include <string>
#include <iostream>
#include <limits>
#include <ccrtp/rtp.h>
#include "atm_communication_sender.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>

#ifdef  CCXX_NAMESPACES
using namespace ost;
using namespace std;
#endif

const int RECEIVER_BASE = 11000; //port do nasluchiwania
class atm_communication: public Thread
{
private:
	RTPSession *socket;
	InetHostAddress local_ip;
	uint32 ssrc;
	
public:
	string host;
	
	atm_communication(string my_ip,string bank_adr)
	{
		local_ip = my_ip.c_str();
		if( ! local_ip )
		{  
			cerr << "Rx: IP address is not correct!" << endl;
			exit();
		}
		socket = new RTPSession(local_ip,RECEIVER_BASE);
		ssrc = socket->getLocalSSRC();
		host=bank_adr;
	}
	~atm_communication()
	{
		terminate();
		delete socket;
	}
	int fd[2];
	
	void run(void)
	{    
		//BANK ADRESS-------
		
		//------------------
		
		socket->setExpireTimeout(3000000);
		socket->setPayloadFormat(StaticPayloadFormat(sptMP2T));
		socket->startRunning();
		
		//Utworzenie dwoch PIPE'ow jeden do odczytu drugi do zapisu
		int fd; 
		char const * myfifo = "./myfifo"; 
		char buf[50];
		mkfifo(myfifo, 0666); 
		int fd2; 
		char const * myfifo2 = "./myfifo2"; 
		char buf2[50];
		mkfifo(myfifo2, 0666); 

		
		
			
		for( int i = 0 ; true ; i++ )
		{

			//Odczytanie PIPE w celu pobrania danych do przeslania do banku
			fd = open(myfifo,O_RDONLY); 
			read(fd, buf, 50); 
			close(fd); 
			
			//Wyslanie zadania do banku
			sendToBank(buf,host);
			
			const AppDataUnit *adu = NULL;
			while ( NULL == adu ) 
			{
				Thread::sleep(10);
				adu = socket->getData(socket->getFirstTimestamp());
			}		
			time_t receiving_time = time(NULL);
			char tmstring[30];
			strftime(tmstring,30,"%X",localtime(&receiving_time));
				
				//Przeksztalcenie otrzymanych danych do postaci tablicy char'ow
			    stringstream buffer;
				buffer<< hex << adu->getData()<<endl;
				string temp = buffer.str();
				strcpy(buf2, temp.c_str());  
				
			    //Przekazanie otrzymanych danych przy pomocy PIPE do procesu obslugi klienta
			    fd2 = open(myfifo2, O_WRONLY); 
				write(fd2, buf2, strlen(buf2)+1); 
				close(fd2); 

			delete adu;
			
		}
	}
	
	void sendToBank(char message[],string adress)
	{//Wyslanie danych pod wskazany adres
			
	char port[]="10000";//Port na ktorym pracuje bank
	char timestamp[]="1";
	char count[]="1";
	//Utworzenie obiektu odpowiedzialnego za utworzenie sesji RTP i wyslanie danych
	Sender sender((unsigned char *)message, InetHostAddress(adress.c_str()),
    atoi(port), atoi(timestamp), atoi(count)); 
	}
};
