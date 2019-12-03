#ifndef __STUDENT__
#define __STUDENT__

#include "printer.h"
#include "nameserver.h"
#include "watcard_office.h"
#include "groupoff.h"
#include "MPRNG.h"
extern MPRNG mprng;

_Task Student {
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    Groupoff &groupoff;
    unsigned int id;
    unsigned int maxPurchases;
    void main();
    
public:
    Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
            unsigned int id, unsigned int maxPurchases );
};

#endif
