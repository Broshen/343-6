#ifndef __NAME__
#define __NAME__

#include "printer.h"
#include "vending.h"

_Task NameServer {
    void main();
    Printer & prt;
    unsigned int numVendingMachines, numStudents;
    unsigned int * assignedMachine; // array of student id : machine mapping
    VendingMachine ** machines;
    
public:
    NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine * vendingmachine );
    VendingMachine * getMachine( unsigned int id );
    VendingMachine ** getMachineList();
};

#endif
