#ifndef __VENDING__
#define __VENDING__

#include "printer.h"
#include "watcard.h"

struct NameServer;


_Task VendingMachine {
	void main();
	Printer &prt;
	NameServer & nameServer;
	unsigned int id, sodaCost;
	unsigned int sodaStock[3] = {0,0,0};

  public:
	enum Flavours { Moist, Human, BBQ  }; 				// flavours of soda (YOU DEFINE)
	_Event Free {};						// free, advertisement
	_Event Funds {};					// insufficient funds
	_Event Stock {};					// out of stock for particular flavour
	VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
	~VendingMachine();
	void buy( Flavours flavour, WATCard & card );
	unsigned int * inventory();
	void restocked();
	_Nomutex unsigned int cost() const;
	_Nomutex unsigned int getId() const;
};

#endif
