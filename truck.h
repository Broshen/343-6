#ifndef __TRUCK__
#define __TRUCK__

#include "printer.h"
#include "nameserver.h"
#include "MPRNG.h"

extern MPRNG mprng;

_Task BottlingPlant;

_Task Truck {
	void main();
	Printer & prt;
	NameServer & nameServer;
	BottlingPlant & plant;
	unsigned int numVendingMachines;
	unsigned int maxStockPerFlavour;
	unsigned int cargo[4]; // 4 flavours
	unsigned int lastStocked = 0;
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif