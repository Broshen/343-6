#ifndef __VENDING__
#define __VENDING__

#include "printer.h"
#include "watcard.h"
#include "MPRNG.h"
extern MPRNG mprng;

_Task NameServer;

_Task VendingMachine {
    void main();
    Printer &prt;
    NameServer & nameServer;
    unsigned int id, sodaCost;
    unsigned int sodaStock[4] = {0,0,0,0};
    bool restocking = false;
    uCondition restockingCond;
    
public:
    enum Flavours { Moist, BBQ, Human, Meat }; 				// flavours of soda (YOU DEFINE)
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
