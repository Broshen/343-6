#ifndef __STUDENT__
#define __STUDENT__

#include "printer.h"
#include "nameserver.h"
struct WATCardOffice; // replace with #include "watcard_office.h" once implemented
#include "groupoff.h"

_Task Student {
	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
			 unsigned int id, unsigned int maxPurchases );
};

#endif