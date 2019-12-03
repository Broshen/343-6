#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottling.h"

void Truck::main(){
	prt.print(Printer::Truck, 'S');
	// The truck begins by obtaining the location of each vending machine from the name server.
	VendingMachine ** machines = nameServer.getMachineList();

	while(true){
		// Before each shipment from the bottling plant, the truck yields a random number of times [1, 10]
		yield(mprng(1,10));

		// The truck then calls BottlingPlant::getShipment to obtain a new shipment of soda;
		// any soda still on the truck is thrown away as it is past its due date
		cargo[0] = 0;
		cargo[1] = 0;
		cargo[2] = 0;
		cargo[3] = 0;

		try {
			plant.getShipment(cargo);
			prt.print(Printer::Truck, 'P', cargo[0]+cargo[1]+cargo[2]+cargo[3]);
		} catch (BottlingPlant::Shutdown &){
			//  If the bottling plant is closing down, the truck terminates.
			break;
		}

		// stock all machines until there is no more soda on the truck or the
		// truck has made a complete cycle of all the vending machines
		for(unsigned int i=0; i<numVendingMachines; i++){
			if(cargo[0] == 0 && cargo[1] == 0 && cargo[2] == 0 && cargo[3] == 0){
				lastStocked = (lastStocked + i)%numVendingMachines;
				break;
			}

			// To ensure fairness, the vending machines are restocked in cyclic order starting at
			// the vending machine after the last machine the truck restocked
			unsigned int idx = (lastStocked + i) % numVendingMachines;


			prt.print(Printer::Truck, 'd', idx, cargo[0]+cargo[1]+cargo[2]+cargo[3]);

			// The truck calls inventory to return a pointer to an array containing the amount
			// of each kind of soda currently in the vending machine.
			unsigned int unfilled = 0; // keep track of number of bottles not replenished
			unsigned int * inv = machines[idx]->inventory();
			for(int i=0; i<4 ; i++){
				// The truck can only restock up to MaxStockPerFlavour for each flavour in each vending machine
				unsigned int amt = std::min(cargo[i], maxStockPerFlavour - inv[i]);
				cargo[i] -= amt;
				inv[i] += amt;

				unfilled += maxStockPerFlavour - inv[i];
			}

			if (unfilled > 0){
				prt.print(Printer::Truck, 'U', idx, unfilled);
			}

			// After transferring new soda into the machine by directly modifying the array passed
			// from inventory, the truck calls restocked to indicate the operation is complete.
			prt.print(Printer::Truck, 'D', idx, cargo[0]+cargo[1]+cargo[2]+cargo[3]);
			machines[idx]->restocked();



		}
		// note: there is no need to update lastStocked if we make a full stocking cycle

	}
	prt.print(Printer::Truck, 'F');
}

Truck::Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
	   unsigned int numVendingMachines, unsigned int maxStockPerFlavour ):
prt{prt},nameServer{nameServer},plant{plant},
numVendingMachines{numVendingMachines},maxStockPerFlavour{maxStockPerFlavour}{

}

