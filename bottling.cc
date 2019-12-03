#include "bottling.h"

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments ):
prt(prt), nameServer(nameServer),numVendingMachines(numVendingMachines),maxShippedPerFlavour(maxShippedPerFlavour),
maxStockPerFlavour(maxStockPerFlavour),timeBetweenShipments(timeBetweenShipments) {}

// The truck calls getShipment to obtain a shipment from the plant (i.e., the production run), and the shipment
// is copied into the cargo array passed by the truck. getShipment throws the exception Shutdown if the bottling
// plant is closing down, and no shipment is copied into the cargo array passed by the truck. A flag variable is
// necessary to know when to raise Shutdown on the correct task stack.)
void BottlingPlant::getShipment( unsigned int cargo[] ){
    if (closing) {
        throw BottlingPlant::Shutdown();
    }
    
    prt.print(Printer::BottlingPlant, 'P');
    cargo[0] += inventory[0];
    cargo[1] += inventory[1];
    cargo[2] += inventory[2];
    cargo[3] += inventory[3];
}

void BottlingPlant::produce(){
    // To simulate a production run of soda, the bottling plant yields for TimeBetweenShipments times (not random).
    yield(timeBetweenShipments);
    inventory[0] = mprng(maxShippedPerFlavour);
    inventory[1] = mprng(maxShippedPerFlavour);
    inventory[2] = mprng(maxShippedPerFlavour);
    inventory[3] = mprng(maxShippedPerFlavour);
    prt.print(Printer::BottlingPlant, 'G', inventory[0]+inventory[1]+inventory[2]+inventory[3]);
}

void BottlingPlant::main(){
    prt.print(Printer::BottlingPlant, 'S');
    // begin by creating a truck, performing a production run, and waiting for the truck to pickup the production run.
    truck = new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour);
    produce();
    
    for(;;) {
        // The bottling plant does not start another production run until the truck has picked up the current run.
        _Accept(getShipment){
            produce();
        } or _Accept(~BottlingPlant) {
            closing = true;
            // accept one last getShipment call so that the Shutdown() exception is throw, and the truck
            // can catch it and exit
            try {
                _Accept(getShipment){}
            } catch( uMutexFailure::RendezvousFailure& ) {}
            break;
        } ;
    }
    prt.print(Printer::BottlingPlant, 'F');
    
}

BottlingPlant::~BottlingPlant(){
    if (truck){
        delete truck;
    }
}

