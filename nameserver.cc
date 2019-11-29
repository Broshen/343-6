#include "nameserver.h"
#include "vending.h"
#include "printer.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents ):
prt{prt},numVendingMachines{numVendingMachines},numStudents{numStudents}{
	machines = new VendingMachine*[numVendingMachines];
	prt.print(Printer::NameServer, 'S');
}

NameServer::~NameServer(){
	prt.print(Printer::NameServer, 'F');
	delete machines;
}

void NameServer::VMregister( VendingMachine * vendingmachine ){
	machines[vendingmachine->getId()] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ){
	return machines[id];
}

VendingMachine ** NameServer::getMachineList(){
	return machines;
}

void NameServer::main(){

}