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
#include "MPRNG.h"

MPRNG mprng;

_Task longAssLoop{
    void main(){
        for(int i=0; i<15; i++){
            yield(15);
        }
    }
};

int main( int argc, char * argv[] ) {
    // MUST BE INT (NOT UNSIGNED) TO CORRECTLY TEST FOR NEGATIVE VALUES
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
    ConfigParms configParms;
    processConfigFile(filename.c_str(), configParms);

    Printer printer(configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers); 

    NameServer * ns = new NameServer(printer, configParms.numVendingMachines, configParms.numStudents);
    VendingMachine **vms = new VendingMachine*[configParms.numVendingMachines];

    for(unsigned int i=0; i<configParms.numVendingMachines; i++){
        vms[i] = new VendingMachine(printer, *ns, i, configParms.sodaCost);
        ns->VMregister(vms[i]);
    }


    BottlingPlant * bp = new BottlingPlant(
        printer, *ns,
        configParms.numVendingMachines,
        configParms.maxShippedPerFlavour,
        configParms.maxStockPerFlavour,
        configParms.timeBetweenShipments
    );


    longAssLoop();

    delete bp;
    delete ns;
    for(unsigned int i=0; i<configParms.numVendingMachines; i++){
        delete vms[i];
    }
    delete vms;
    
} // main
