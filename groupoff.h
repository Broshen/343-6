#ifndef __GROUPOFF__
#define __GROUPOFF__

_Task Groupoff {
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard();
};

#endif

