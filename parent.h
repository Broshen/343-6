#ifndef __PARENT__
#define __PARENT__

#include "printer.h"
#include "bank.h"

_Task Parent {
	Printer &prt;
	Bank &bank;
	unsigned int numStudents, parentalDelay;
	void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif