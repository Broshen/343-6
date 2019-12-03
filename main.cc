#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
#include "config.h"
#include "printer.h"
#include "nameserver.h"
#include "vending.h"
#include "bottling.h"
#include "watcard_office.h"
#include "parent.h"
#include "student.h"
#include "MPRNG.h"

MPRNG mprng;

int main( int argc, char * argv[] ) {
    unsigned int seed = getpid();
    string filename = "soda.config";
    
    try {
        switch ( argc ) {
            case 3:
                seed = stoi(argv[2]); if (seed <= 0 ) throw 1; // invalid ?
                // FALL THROUGH
            case 2:
                filename = argv[1];
                // FALL THROUGH
            case 1:					// defaults
                break;
            default:					// wrong number of options
                throw 1;
        } // switch
    } catch( ... ) {
        cerr << "Usage: " << argv[0]
        << " [ config-file [ Seed ] ]" << endl;
        exit( EXIT_FAILURE );				// TERMINATE
    } // try

    mprng.set_seed(seed);

    // The program main starts by calling processConfigFile to read and parse the simulation configurations.
    ConfigParms configParms;
    processConfigFile(filename.c_str(), configParms);

    // Create in order the printer, bank, parent, WATCard office, groupoff, name server, vending machines, bottling plant,
    // and students.
    Printer printer(configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers); 
    Bank bank(configParms.numStudents);
    Parent * parent = new Parent(printer, bank, configParms.numStudents, configParms.parentalDelay);

    // Couriers are created by the WATCard office.
    WATCardOffice *office = new WATCardOffice(printer, bank, configParms.numCouriers);
    Groupoff *groupoff = new Groupoff(printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay);

    NameServer * ns = new NameServer(printer, configParms.numVendingMachines, configParms.numStudents);
    VendingMachine **vms = new VendingMachine*[configParms.numVendingMachines];

    for(unsigned int i=0; i<configParms.numVendingMachines; i++){
        vms[i] = new VendingMachine(printer, *ns, i, configParms.sodaCost);
    }

    // Truck is created by the bottling plant
    BottlingPlant * bp = new BottlingPlant(
        printer, *ns,
        configParms.numVendingMachines,
        configParms.maxShippedPerFlavour,
        configParms.maxStockPerFlavour,
        configParms.timeBetweenShipments
    );

    Student **students = new Student*[configParms.numStudents];

    for (unsigned int i = 0; i < configParms.numStudents; i ++) {
        students[i] = new Student(printer, *ns, *office, *groupoff, i, configParms.maxPurchases);
    }

    // wait for all students have purchased their specified number of bottles before terminating
    for (unsigned int i = 0; i < configParms.numStudents; i ++) {
       delete students[i];
    }
    
    delete [] students;
    delete parent;
    delete office;
    delete groupoff;

    // delete the bottling plant before deleting the vending machines to allow the truck to complete its final
    // deliveries to the vending machines;
    delete bp;
    delete ns;
    for(unsigned int i=0; i<configParms.numVendingMachines; i++){
        delete vms[i];
    }
    delete [] vms;
    
} // main
