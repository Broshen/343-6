#ifndef __GROUPOFF__
#define __GROUPOFF__

#include "printer.h"
#include "watcard.h"

_Task Groupoff {
	Printer &prt;
	unsigned int numStudents, sodaCost, groupoffDelay, waiting = 0;

	WATCard::FWATCard *giftCards; // list of futures to deliver to students
	WATCard **fulfilled; // list of giftcards that have been delivered
	int *studentIndex; // list of student indices still waiting to receive giftcards

  void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard();
	~Groupoff();
};

#endif
