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
enum TRADETYPE
{
	BUY=0,SELL
};

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
	return (TradeType == TRADETYPE::BUY ? ("BUY") : ("SELL"));
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
	return (TradeType == "BUY" ? (TRADETYPE::BUY) : (TRADETYPE::SELL));
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
}

inline void book_sell_(string sym, int price, int size)
{
	cout << "Book_Sell " << sym << " " << price << " " << size << endl;
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
			itype = TRADETYPE::BUY;
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
			itype = TRADETYPE::SELL;
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