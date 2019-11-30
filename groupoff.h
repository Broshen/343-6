#ifndef __GROUPOFF__
#define __GROUPOFF__

#include "printer.h"
#include "watcard.h"

_Task Groupoff {
	Printer &prt;
	unsigned int numStudents, sodaCost, groupoffDelay, waiting = 0;
	void main();
	WATCard::FWATCard *giftCards;
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard();
	~Groupoff();
};

#endif

