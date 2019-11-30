#include "vending.h"
#include "printer.h"

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ):
prt{prt}, nameServer{nameServer}, id{id}, sodaCost{sodaCost} {
	prt.print(Printer::Vending, id, 'S', sodaCost);
}

VendingMachine::~VendingMachine(){
	prt.print(Printer::Vending, id, 'F');
}

// A student calls buy to obtain one of their favourite sodas.
// TODO: A flag variable is necessary to know when to raise Funds, Stock or Free on the correct task stack?
void VendingMachine::buy(VendingMachine::Flavours flavour, WATCard & card ){
	// Check first if the student has sufficient funds to purchase the soda
	if (card.getBalance() < sodaCost) {
		throw VendingMachine::Funds();
	}

	// Check if the specified soda is available
	if (sodaStock[flavour] == 0) {
		throw VendingMachine::Stock();
	}

	// Buy soda
	sodaStock[flavour] -= 1;
	prt.print(Printer::Vending, id, 'B', flavour, sodaStock[flavour]);

	// Once a purchase is possible, there is a 1 in 5 chance the soda is free, if the student watches
	// an advertisement, which is indicated by raising exception Free.
	if (mprng(4) == 4){
		throw VendingMachine::Free();
	} else {
		card.withdraw(sodaCost);
	}
}

unsigned int * VendingMachine::inventory(){
	return (unsigned int * )sodaStock;
}
void VendingMachine::restocked(){

}
_Nomutex unsigned int VendingMachine::cost() const{
	return sodaCost;

}
_Nomutex unsigned int VendingMachine::getId() const{
	return id;
}


void VendingMachine::main() {

}