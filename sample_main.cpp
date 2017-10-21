/* How to compile and run this file:
   COMPILATION: g++ -o bot.exe bot.cpp
   RUNNING: while true; do ./bot.exe; sleep 1; done
*/

/* C includes for networking things */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

/* C++ includes */
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <sstream>
//#include"explainer.h"
/* The Configuration class is used to tell the bot how to connect
   to the appropriate exchange. The `test_exchange_index` variable
   only changes the Configuration when `test_mode` is set to `true`.
*/
class Configuration {
private:
  /*
    0 = prod-like
    1 = slower
    2 = empty
  */
  static int const test_exchange_index = 0;
public:
  std::string team_name;
  std::string exchange_hostname;
  int exchange_port;
  Configuration(bool test_mode) : team_name("RIYUESHENJIAO"){
    exchange_port = 20000; /* Default text based port */
    if(true == test_mode) {
      exchange_hostname = "test-exch-" + team_name;
      exchange_port += test_exchange_index;
    } else {
      exchange_hostname = "localhost";
    }
  }
};

/* Connection establishes a read/write connection to the exchange,
   and facilitates communication with it */
class Connection {
private:
  FILE * in;
  FILE * out;
  int socket_fd;
public:
  Connection(Configuration configuration){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      throw std::runtime_error("Could not create socket");
    }
    std::string hostname = configuration.exchange_hostname;
    hostent *record = gethostbyname(hostname.c_str());
    if(!record) {
      throw std::invalid_argument("Could not resolve host '" + hostname + "'");
    }
    in_addr *address = reinterpret_cast<in_addr *>(record->h_addr);
    std::string ip_address = inet_ntoa(*address);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip_address.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(configuration.exchange_port);

    int res = connect(sock, ((struct sockaddr *) &server), sizeof(server));
    if (res < 0) {
      throw std::runtime_error("could not connect");
    }
    FILE *exchange_in = fdopen(sock, "r");
    if (exchange_in == NULL){
      throw std::runtime_error("could not open socket for writing");
    }
    FILE *exchange_out = fdopen(sock, "w");
    if (exchange_out == NULL){
      throw std::runtime_error("could not open socket for reading");
    }

    setlinebuf(exchange_in);
    setlinebuf(exchange_out);
    this->in = exchange_in;
    this->out = exchange_out;
    this->socket_fd = res;
  }

  /** Send a string to the server */
  void send_to_exchange(std::string input) {
    std::string line(input);
    /* All messages must always be uppercase */
    std::transform(line.begin(), line.end(), line.begin(), ::toupper);
    int res = fprintf(this->out, "%s\n", line.c_str());
    if (res < 0) {
      throw std::runtime_error("error sending to exchange");
    }
  }

  /** Read a line from the server, dropping the newline at the end */
  std::string read_from_exchange()
  {
    /* We assume that no message from the exchange is longer
       than 10,000 chars */
    const size_t len = 10000;
    char buf[len];
    if(!fgets(buf, len, this->in)){
      throw std::runtime_error("reading line from socket");
    }

    int read_length = strlen(buf);
    std::string result(buf);
    /* Chop off the newline */
    result.resize(result.length() - 1);
    return result;
  }
};

/** Join a vector of strings together, with a separator in-between
    each string. This is useful for space-separating things */
std::string join(std::string sep, std::vector<std::string> strs) {
  std::ostringstream stream;
  const int size = strs.size();
  for(int i = 0; i < size; ++i) {
    stream << strs[i];
    if(i != (strs.size() - 1)) {
      stream << sep;
    }
  }
  return stream.str();
}

#ifndef EXPLAINER_H
#define EXPLAINER_H

#include<iostream>
#include<string>
#include<vector>
#include<sstream>

using std::string;
using std::vector;
//using std::streamstream;
using namespace std;
//enum TRADETYPE
//{
//	BUY=0,SELL
//};
const int TRADETYPE_BUY=0;
const int TRADETYPE_SELL=1;
int tid;

//cmd gen
const string TEAMNAME = "RiYueShenJiao";

string _hello()
{
	string rtn = "HELLO " + TEAMNAME;
	return rtn;
}

inline string __itos(int val)
{
	stringstream ss;
	string rtn;
	rtn.clear();
	ss << val;
	ss >> rtn;
	return rtn;
}

inline string __itos_type(int TradeType)
{
	return (TradeType == TRADETYPE_BUY ? ("BUY") : ("SELL"));
}

inline int __stoi(string s)
{
	int rtn = 0; bool flag = false;
	for (string::iterator it = s.begin(); it != s.end(); ++it)
	{
		if ((*it) == '-') flag = true;
		else rtn = rtn * 10 + (*it) - '0';
	}
	return flag ? (-rtn) : rtn;
}

inline int __stoi_type(string TradeType)
{
	return (TradeType == "BUY" ? (TRADETYPE_BUY) : (TRADETYPE_SELL));
}

string _add(int id, string sym, int TradeType, int price, int size)
{
	string rtn = "ADD "+__itos(id)+" "+sym+" "+__itos_type(TradeType)+" "+__itos(price)+" "+__itos(size);
	return rtn;
}

string _convert(int id, string sym, int TradeType, int size)
{
	string rtn = "CONVERT " + __itos(id) + " " + sym + " " + __itos_type(TradeType) + " " + __itos(size);
	return rtn;
}

string _cancel(int id)
{
	string rtn = "CANCEL " + __itos(id);
	return rtn;
}

inline void hello_(string sym, int pos)
{
	cout << "Hello " << sym << " " << pos << endl;
}

inline void open_(string sym)
{
	cout << "Open " << sym << endl;
}

inline void close_(string sym)
{
	cout << "Close " << sym << endl;
}

inline void error_(string msg)
{
	cout << "Error " << msg << endl;
}

inline void book_buy_(string sym, int price, int size)
{
	cout << "Book_Buy " << sym << " " << price << " " << size << endl;
    if(sym=="USDHKD")
    {
        string cmd;
        if(price>=80000)
        {
            cmd=_add(++tid,sym,TRADETYPE_SELL,80000,size);
            conn.send_to_exchange(cmd);
        }
    }
}

inline void book_sell_(string sym, int price, int size)
{
	cout << "Book_Sell " << sym << " " << price << " " << size << endl;
	if(sym=="USDHKD")
	{
		string cmd;
		if(price<=80000)
		{
			cmd=_add(++tid,sym,TRADETYPE_BUY,80000,size);
			conn.send_to_exchange(cmd);
		}
	}
}

inline void trade_(string sym, int price, int size)
{
	cout << "Trade " << sym << " " << price << " " << size << endl;
}

inline void ask_(int id)
{
	cout << "Ask " << id << endl;
}

inline void reject_(int id, string msg)
{
	cout << "Reject " << id << " " << msg << endl;
}

inline void fill_(int id, string sym, int TradeType, int price, int size)
{
	cout << "Fill " << sym << " " << TradeType << " " << price << " " << size << endl;
}

inline void out_(int id)
{
	cout << "Out " << id << endl;
}

inline void _cmd_explainer(string cmd)
{
	cmd+=' ';
	string temp, sym, stype;
	int itype, wsize;
	vector<string> wordlist;
	int i, p;
	string info, para1, para2;

	wordlist.clear();
	for (string::iterator it = cmd.begin(); it != cmd.end(); ++it)
	{
		if ((*it) == ' '||(*it)=='\n')
		{
			wordlist.push_back(temp);
			temp.clear();
			continue;
		}
		temp += (*it);
	}
	wsize = wordlist.size();
	if (wordlist[0] == "TRADE")
	{
		trade_(wordlist[1], __stoi(wordlist[2]), __stoi(wordlist[3]));
	}
	else if (wordlist[0] == "BOOK")
	{
		sym = wordlist[1];
		//int i = 2, p;
		i = 2;
	//	string info, sprice, ssize;
		if (wordlist[i] == "BUY")
		{
			itype = TRADETYPE_BUY;
			for (++i; i < wsize&&wordlist[i] != "SELL"; ++i)
			{
				info = wordlist[i];
				p = info.find(":");
				para1 = info.substr(0, p);
				para2 = info.substr(p + 1, info.size() - p - 1);

				book_buy_(sym, __stoi(para1), __stoi(para2));
			}
		}
		if (i < wsize)
		{
			itype = TRADETYPE_SELL;
			for (++i; i < wsize; ++i)
			{
				info = wordlist[i];
				p = info.find(":");
				para1 = info.substr(0, p);
				para2 = info.substr(p + 1, info.size() - p - 1);

				book_sell_(sym, __stoi(para1), __stoi(para2));
			}
		}
	}
	else if (wordlist[0] == "FILL")
	{
		fill_(__stoi(wordlist[1]), wordlist[2], __stoi_type(wordlist[3]), __stoi(wordlist[4]), __stoi(wordlist[5]));
	}
	else if (wordlist[0] == "ASK")
	{
		ask_(__stoi(wordlist[1]));
	}
	else if (wordlist[0] == "OPEN")
	{
		for (i = 1; i < wsize; ++i)
			open_(wordlist[i]);
	}
	else if (wordlist[0] == "CLOSE")
	{
		for (i = 1; i < wsize; ++i)
			close_(wordlist[i]);
	}
	else if (wordlist[0] == "OUT")
	{
		out_(__stoi(wordlist[1]));
	}
	else if (wordlist[0] == "HELLO")
	{
		for (i = 1; i < wsize; ++i)
		{
			p = wordlist[i].find(":");
			para1 = wordlist[i].substr(0, p);
			para2 = wordlist[i].substr(p + 1, info.size() - p - 1);
			hello_(para1, __stoi(para2));
		}
	}
	else if (wordlist[0] == "REJECT")
	{
		reject_(__stoi(wordlist[1]), wordlist[2]);
	}
	else if (wordlist[0] == "ERROR")
	{
		error_(wordlist[1]);
	}

}
#endif //EXPLAINER_H

int main(int argc, char *argv[])
{
    // Be very careful with this boolean! It switches between test and prod
    bool test_mode = true;
    Configuration config(test_mode);
    Connection conn(config);

    std::vector<std::string> data;
    data.push_back(std::string("HELLO"));
    data.push_back(config.team_name);
    /*
      A common mistake people make is to conn.send_to_exchange() > 1
      time for every conn.read_from_exchange() response.
      Since many write messages generate marketdata, this will cause an
      exponential explosion in pending messages. Please, don't do that!
    */
    conn.send_to_exchange(join(" ", data));
    while(1)
    {
        std::string line = conn.read_from_exchange();
        _cmd_explainer(line);
        //std::cout << line << std::endl;
    }
    return 0;
}
