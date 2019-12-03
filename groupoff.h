#ifndef __GROUPOFF__
#define __GROUPOFF__

#include "printer.h"
#include "watcard.h"

_Task Groupoff {
	Printer &prt;
	unsigned int numStudents, sodaCost, groupoffDelay, waiting = 0;
	WATCard::FWATCard *giftCards;
	WATCard **fulfilled;
	int *studentIndex;
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard();
	~Groupoff();
};

#endif
