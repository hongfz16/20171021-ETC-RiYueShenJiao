#include"explainer.h"

/*
HELLO X:100 Y:23 Z:0
OPEN ghj ghjk ghj
CLOSE FGHJFGHJ GHJK FGHJ
ERROR FGHJ
BOOK FGH BUY 10:-3 2:4 5:6 SELL 3:4 5:4
BOOK FGH SELL 3:4 5:4
BOOK FGH BUY SELL 3:4 5:4
BOOK FGH BUY 10:3 2:4 5:6 SELL
TRADE GHJ 123 -4322
ASK 124
REJECT 234 GHJKL
FILL 234 FGH BUY -23 421
FILL 234 FGH SELL -23 421
OUT 234

*/
int main()
{
	string cmd;
	cout << _add(123, "ghj", TRADETYPE::BUY, 134, -1324) << endl;
	cout << _hello() << endl;
	cout << _convert(123, "AE", TRADETYPE::SELL, 234) << endl;
	cout << _cancel(-123)<<endl;
	while (true)
	{
		getline(cin, cmd);
		cmd += ' ';
		_cmd_explainer(cmd);
		cout << endl;
	}
}