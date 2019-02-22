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
	
	atm_communication(string bank_adr){
		local_ip = "127.0.0.1";
		if( ! local_ip ){  
			cerr << "Rx: IP address is not correct!" << endl;
			exit();
		}
		socket = new RTPSession(local_ip,RECEIVER_BASE);
		ssrc = socket->getLocalSSRC();
		host=bank_adr;
	}
	~atm_communication(){
		terminate();
		delete socket;
	}
	int fd[2];
	
	void run(void){    
		//BANK ADRESS-------
		
		//------------------
		
		socket->setExpireTimeout(3000000);
		socket->setPayloadFormat(StaticPayloadFormat(sptMP2T));
		socket->startRunning();
		int fd; 
		char const * myfifo = "./myfifo"; 
		char buf[50];
		mkfifo(myfifo, 0666); 
		
		int fd2; 
		char const * myfifo2 = "./myfifo2"; 
		char buf2[50];
		mkfifo(myfifo2, 0666); 

		
		
			
		for( int i = 0 ; true ; i++ ){

			fd = open(myfifo,O_RDONLY); 
			read(fd, buf, 50); 
			close(fd); 

			sendToBank(buf,host);
			
			const AppDataUnit *adu = NULL;
			while ( NULL == adu ) {
				Thread::sleep(10);
				adu = socket->getData(socket->getFirstTimestamp());
			}		
			time_t receiving_time = time(NULL);
			char tmstring[30];
			strftime(tmstring,30,"%X",localtime(&receiving_time));
			     //OTRZYMANE DANE OD BANKU W ADU->GetData
			    
			    stringstream buffer;
				buffer<< hex << adu->getData()<<endl;
				string temp = buffer.str();
				strcpy(buf2, temp.c_str());  
			     
			    fd2 = open(myfifo2, O_WRONLY); 
				write(fd2, buf2, strlen(buf2)+1); 
				close(fd2); 
				

			     //--------------------------------------
			delete adu;
			
		}
	}
	
	void sendToBank(char message[],string adress){
		
		
	
	char timestamp[]="1";
	char count[]="1";
	
	//ATM ADRESS-------
	char port[]="10000";
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

        
	}
};
