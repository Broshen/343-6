#ifndef __TRUCK__
#define __TRUCK__

#include "printer.h"
#include "nameserver.h"
#include "bottling.h"

_Task Truck {
	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif