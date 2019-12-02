#include "nameserver.h"
#include "vending.h"
#include "printer.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
prt{prt},numVendingMachines{numVendingMachines},numStudents{numStudents}{
	machines = new VendingMachine*[numVendingMachines];

	// logically distribute students evenly across the vending machines in a round-robin fashion.
	// e.g. student id 0 is assigned to the first registered vending-machine, student id 1 is assigned to the second registered vending
	// machine, etc., until there are no more registered vending-machines, and then start again with the first registered vending-machine.
	assignedMachine = new unsigned int[numStudents];
	for(unsigned int i=0; i<numStudents; i++){
		assignedMachine[i] = i % numVendingMachines;
	}
}

NameServer::~NameServer(){
	delete [] assignedMachine;
	delete [] machines;
}

// All vending machines are registered before being given out.
void NameServer::VMregister( VendingMachine * vendingmachine ){
	machines[vendingmachine->getId()] = vendingmachine;
}

// A student calls getMachine to find a vending machine, and the name server must cycle through the vending
// machines separately for each student starting from the initial position via modulo incrementing to ensure a
// student has a chance to visit every machine.
VendingMachine * NameServer::getMachine( unsigned int id ){
	VendingMachine * curMachine =  machines[assignedMachine[id]];

	assignedMachine[id] = (assignedMachine[id] + 1) % numVendingMachines;

	return curMachine;
}

// The truck calls getMachineList to obtain an array of pointers to
// vending machines so it can visit each machine to deliver new soda.
VendingMachine ** NameServer::getMachineList(){
	return machines;
}

void NameServer::main(){
  prt.print(Printer::NameServer, 'S');

 	for (;;){
    _Accept(~NameServer) {
      break;
    } or _Accept(getMachineList, VMregister, getMachine) {}
  }

  prt.print(Printer::NameServer, 'F');
}