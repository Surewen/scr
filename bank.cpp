#include <cstdlib>
#include <sstream>
#include <ccrtp/rtp.h>
#ifdef CCXX_NAMESPACES
using namespace ost;
using namespace std;
#endif
/**
* @brief Klasa wysylajaca
*
* Odpowiedzialna za zestawienie zesji RTP ze wskazanym hostem i przeslanie danych
*/
class Sender : public RTPSession, public TimerPort
{
  public:
	Sender(const unsigned char *data, const InetHostAddress &ia,
		   tpport_t port, uint32 tstamp, uint16 count) : RTPSession(InetHostAddress("0.0.0.0")), packetsPerSecond(10)
	{
		uint32 timestamp = tstamp ? tstamp : 0;
		defaultApplication().setSDESItem(SDESItemTypeTOOL, "rtpsend demo app.");
		setSchedulingTimeout(10000);
		setExpireTimeout(1000000);

		if (!addDestination(ia, port))
		{
			cerr << "Could not connect" << endl;
			exit();
		}
		setPayloadFormat(StaticPayloadFormat(sptPCMU));
		startRunning();
		uint16 tstampInc = getCurrentRTPClockRate() / packetsPerSecond;
		uint32 period = 1000 / packetsPerSecond;
		TimerPort::setTimer(period);
		for (int i = 0; i < count; i++)
		{
			putData(timestamp + i * tstampInc,
					data, strlen((char *)data) + 1);
			Thread::sleep(TimerPort::getTimer());
			TimerPort::incTimer(period);
		}
	}

  private:
	const uint16 packetsPerSecond;
};
/**
 * Metoda do przeslania danych na podany adres IP.
 *
 * @param message Wiadomosc do przeslania w postaci tablicy znakow.
 * @param address Adres ip, do ktorego ma zostac wyslana wiadomosc.
 */
void send(char message[], string adress)
{

	char port[] = "11000"; //Port na ktorym pracuja bankomaty
	char timestamp[] = "10";
	char count[] = "1";
	//Utworzenie obiektu odpowiedzialnego za utworzenie sesji RTP i wyslanie danych
	Sender sender((unsigned char *)message, InetHostAddress(adress.c_str()),
				  atoi(port), atoi(timestamp), atoi(count));
}
/**
* @brief Klasa nasluchujaca
*
* Odpowiedzialna za zestawienie nasluchiwanie przychodzacych informacji.
*/
class Listener : RTPSession
{
  public:
	ifstream accounts;
	ifstream log;

	string card_number = "";
	string pin;
	string balance;
	bool account_exists = false;

	Listener(InetMcastAddress &ima, tpport_t port) : RTPSession(ima, port) {}

	Listener(InetHostAddress &ia, tpport_t port) : RTPSession(ia, port) {}
	/**
	 * Metoda odpowiedzialna za skladowanie logow.
	 *
	 * @param log_line Jednoliniowy string z zapisem calej transakcji.
	 */
	void add_log(string log_line)
	{
		log.open("log.txt");
		string line;
		string full_file;
		while (log.good())
		{
			getline(log, line, '\n');
			full_file.append(line);
			full_file.append("\n");
		}
		full_file.append(log_line);

		log.close();
		ofstream out_file("log.txt");
		out_file << full_file;
	}
	/**
	 * Metoda odpowiedzialna za pobranie numeru PIN konta.
	 *
	 * @param account_number Numer karty klienta.
	 */
	string get_pin(string account_number)
	{
		accounts.open("accounts.csv");
		while (accounts.good())
		{
			getline(accounts, card_number, ',');
			getline(accounts, pin, ',');
			getline(accounts, balance, '\n');
			if (account_number == card_number)
			{
				account_exists = true;
				break;
			}
		}
		accounts.close();
		if (account_exists)
		{
			return pin;
		}
		else
		{
			return "0";
		}
	}
	/**
	 * Metoda odpowiedzialna za pobranie stanu konta.
	 *
	 * @param account_number Numer karty klienta.
	 */
	string get_balance(string account_number)
	{
		accounts.open("accounts.csv");
		while (accounts.good())
		{
			getline(accounts, card_number, ',');
			getline(accounts, pin, ',');
			getline(accounts, balance, '\n');
			if (account_number == card_number)
			{
				account_exists = true;
				break;
			}
		}
		accounts.close();
		if (account_exists)
		{
			return balance;
		}
		else
		{
			return "0";
		}
	}
	/**
	 * Metoda odpowiedzialna za pobranie zadanej kwoty z konta.
	 *
	 * @param account_number Numer karty klienta.
	 * @param account_number Kwota do pobrania.
	 */
	bool substract_balance(string account_number, int amount)
	{
		bool flag = false;
		string full_file;
		accounts.open("accounts.csv");
		while (accounts.good())
		{
			getline(accounts, card_number, ',');
			if (card_number == "")
				break;
			full_file.append(card_number);
			full_file.append(",");
			getline(accounts, pin, ',');
			full_file.append(pin);
			full_file.append(",");
			getline(accounts, balance, '\n');
			if (card_number == account_number)
			{
				if (stoi(balance) < amount)
				{
					flag = false;
				}
				else
				{
					balance = to_string(stoi(balance) - amount);
					flag = true;
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
	/**
	 * Metoda odpowiedzialna za dodanie zadanej kwoty do konta.
	 *
	 * @param account_number Numer karty klienta.
	 * @param account_number Kwota do pobrania.
	 */
	void add_balance(string account_number, int amount)
	{
		string full_file;
		accounts.open("accounts.csv");
		while (accounts.good())
		{
			getline(accounts, card_number, ',');
			if (card_number == "")
				break;
			full_file.append(card_number);
			full_file.append(",");
			getline(accounts, pin, ',');
			full_file.append(pin);
			full_file.append(",");
			getline(accounts, balance, '\n');
			if (card_number == account_number)
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
	/**
	 * Metoda odpowiedzialna za wykoanie przelewu z jednego konta do drugiego.
	 *
	 * @param sender Numer karty klienta wysylajacego.
	 * @param receiver Numer karty klienta odbierajacego.
	 * @param account_number Kwota do przeslania.
	 */
	bool transfer(string sender, string receiver, int amount)
	{
		if (substract_balance(sender, amount))
		{
			add_balance(receiver, amount);
			return true;
		}
		else
		{
			return false;
		}
	}

	void listen()
	{
		defaultApplication().setSDESItem(SDESItemTypeTOOL, "rtplisten demo app.");
		setExpireTimeout(1000000);
		setPayloadFormat(StaticPayloadFormat(sptPCMU));
		startRunning();

		char message[50];		//Zmienna przechowujaca tworzona wiadomosc dla bankomatu
		stringstream ad_buffer; //Bufor przechowujacy adres IP bankomatu ktoremu nalezy odpowiedziec
		string adresstring;		//String z w.w. adresem

		for (;;)
		{
			const AppDataUnit *adu;
			while ((adu = getData(getFirstTimestamp())))
			{

				//Pobranie adresu IP bankomatu wysylajacego zadanie do banku
				ad_buffer.clear();
				ad_buffer.str(string());
				ad_buffer << adu->getSource().getNetworkAddress();
				adresstring = ad_buffer.str();

				//sstream zawierajacy linijke ktora zostanie wpisana do logu/wyswietlona na terminalu banku
				stringstream loglinestream;
				loglinestream << "Request from: "
							  << dec << adu->getSource().getNetworkAddress()
							  << ":"
							  << adu->getSource().getDataTransportPort() << " ";
				string logline = loglinestream.str();

				//bufor dla danych przesylanych przez bankomat przy uzyciu RTP
				stringstream buffer;
				buffer << hex << adu->getData() << endl;
				string temp = buffer.str(); //tymczasowy string z ktorego wybierane beda skladowe zawarte w rozkazie

				//zmienne pomocnicze do przechowywania danych
				char command_buf[1];
				string account_number;
				string amount;
				string account_number2;

				//wybranie pierwszej skladowej (numer rozkazu)
				size_t pos = 0;
				string delimiter = ":";
				string command_string = temp.substr(0, temp.find(delimiter));
				command_string.substr(pos + delimiter.length());
				temp.erase(0, temp.find(delimiter) + delimiter.length());
				strcpy(command_buf, command_string.c_str());

				//dzialania na podstawie numeru rozkazu
				if (command_buf[0] == '1')
				{ //AUTORYZACJA

					//Wybranie z przeslanych danych numeru konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());
					loglinestream << "PIN request for account: " + account_number + ". Sending PIN: " + get_pin(account_number) << endl;
					logline = loglinestream.str();
					//Odeslanie PIN o danym numerze konta
					string data = "2:";
					data = data + get_pin(account_number) + string(":");
					strcpy(message, data.c_str());
					//-----------------------------------
				}
				else if (command_buf[0] == '3')
				{ //SALDO

					//Wybranie z przeslanych danych numeru konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wstawienie danych do zmiennej tworzacej log
					loglinestream << "Request balance for account: " + account_number + " Sending current balance: " + get_balance(account_number) << endl;
					logline = loglinestream.str();

					//Odeslanie salda o danym numerze konta
					string data = "4:";
					data = data + account_number + string(":") + get_balance(account_number) + string(":");
					strcpy(message, data.c_str());
					//-----------------------------------
				}
				else if (command_buf[0] == '5')
				{ //WYPLATA

					//Wybranie z przeslanych danych numeru konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wybranie z przeslanych danych kwoty
					pos = temp.find(delimiter);
					amount = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wstawienie danych do zmiennej tworzacej log
					loglinestream << "Withdrawal request for account: " + account_number + " Amount: " + amount << endl;
					logline = loglinestream.str();

					//Odeslanie potwierdzenia operacji
					if (substract_balance(account_number, stoi(amount)))
					{
						//Wstawienie do danych do zmiennej tworzacej log
						loglinestream << "Operation success. Sending approval." << endl;
						logline = loglinestream.str();
						cout << logline << endl;

						//Operacja udana
						string data = "7:";
						strcpy(message, data.c_str());
					}
					else
					{

						//Wstawienie do danych do zmiennej tworzacej log
						loglinestream << "Operation failed. Not enough funds." << endl;
						logline = loglinestream.str();
						cout << logline << endl;

						//Operacja nieudana
						string data = "8:";
						strcpy(message, data.c_str());
					}

					//-----------------------------------
				}
				else if (command_buf[0] == '6')
				{ //WPLATA

					//Wybranie z przeslanych danych numeru konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wybranie z przeslanych danych kowty
					pos = temp.find(delimiter);
					amount = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wstawienie danych do zmiennej tworzacej log
					loglinestream << "Deposit request for account: " + account_number + " Amount: " + amount << endl;
					logline = loglinestream.str();

					//Wykonanie operacji
					add_balance(account_number, stoi(amount));

					//Wstawienie danych do zmiennej tworzacej log
					loglinestream << "Operation success. Sending approval." << endl;

					logline = loglinestream.str();
					//Odeslanie potwierdzenia operacji
					string data = "7:";
					strcpy(message, data.c_str());
				}
				else if (command_buf[0] == '9')
				{ //PRZELEW

					//Wybranie z przeslanych danych numeru konta
					size_t pos = 0;
					pos = temp.find(delimiter);
					account_number = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wybranie z przeslanych danych numeru konta docelowego
					pos = temp.find(delimiter);
					account_number2 = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wybranie z przeslanych danych kwoty
					pos = temp.find(delimiter);
					amount = temp.substr(0, pos);
					temp.erase(0, pos + delimiter.length());

					//Wstawienie danych do zmiennej tworzacej log
					loglinestream << "Transfer request for account: " + account_number + " to: " + account_number2 + " Amount: " + amount << endl;
					logline = loglinestream.str();

					//Wykonanie operacji przelewu
					if (transfer(account_number, account_number2, stoi(amount)))
					{

						//Wstawienie danych do zmiennej tworzacej log
						loglinestream << "Operation success. Sending approval." << endl;
						logline = loglinestream.str();

						//Operacja udana
						string data = "7:";
						strcpy(message, data.c_str());
					}
					else
					{

						//Wstawienie danych do zmiennej tworzacej log
						loglinestream << "Operation failed. Not enough funds." << endl;
						logline = loglinestream.str();

						//Operacja nieudana
						string data = "8:";
						strcpy(message, data.c_str());
					}
				}
				//Wypisanie na ekran linijki logu oraz wstawienie do pliku log.txt
				cout << logline;
				add_log(logline);

				//Wyslanie do bankomatu odpowiedzi
				send(message, adresstring);
				delete adu;
			}
			Thread::sleep(7);
		}
	}
};
/**
	 * Metoda odpowiedzialna za wykoanie utworzenie obiektu nasłuchującego.
	 *
	 * @param myip Adres ip banku.
	 */
void startListening(string myip)
{
	//Utworzenie odpowiedniego listenera
	InetMcastAddress ima;
	try
	{
		ima = InetMcastAddress(myip.c_str());
	}
	catch (...)
	{
	}

	Listener *foo;
	tpport_t port = atoi("10000");
	if (ima.isInetAddress())
	{
		foo = new Listener(ima, port);
	}
	else
	{
		InetHostAddress ia(myip.c_str());
		foo = new Listener(ia, atoi("10000"));
	}
	foo->listen();
	delete foo;
}
int main(int argc, char *argv[])
{

	string host;

	if (argc > 1)
	{
		//Adres IP ktory ma byc nasluchiwany
		host = argv[1];
		//Uruchomienie procesu banku
		startListening(host);
	}

	return 0;
}
