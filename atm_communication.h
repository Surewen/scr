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
	atm_communication(){
		local_ip = "127.0.0.1";
		if( ! local_ip ){  
			cerr << "Rx: IP address is not correct!" << endl;
			exit();
		}
		socket = new RTPSession(local_ip,RECEIVER_BASE);
		ssrc = socket->getLocalSSRC();
	}
	~atm_communication(){
		terminate();
		delete socket;
	}
	int fd[2];
	
	void run(void){    
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


		char host[]="127.0.0.1";
		char port[]="10000";
			
		for( int i = 0 ; true ; i++ ){

			fd = open(myfifo,O_RDONLY); 
			read(fd, buf, 50); 
			close(fd); 

			sendToBank(buf,host,port);
			
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
	void sendToBank(char data[],char host[],char port[]){
	
	char timestamp[]="1";
	char count[]="1";
	
    Sender sender((unsigned char *)data, InetHostAddress(host),
        atoi(port), atoi(timestamp), atoi(count));
	}
};
