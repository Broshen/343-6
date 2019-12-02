#ifndef __BOTTLING__
#define __BOTTLING__

#include <uSemaphore.h>

#include "printer.h"
#include "nameserver.h"
#include "truck.h"
#include "MPRNG.h"

extern MPRNG mprng;

_Task BottlingPlant {
	void main();
	Printer & prt;
	NameServer & nameServer;
	unsigned int numVendingMachines,maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments;
	Truck * truck;
	unsigned int inventory[4]; // inventory for each flavor
	bool closing = false;
	void produce();

  public:
	_Event Shutdown {};					// shutdown plant
	BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
				 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
				 unsigned int timeBetweenShipments );
	~BottlingPlant();
	void getShipment( unsigned int cargo[] );
};

#endif