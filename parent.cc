#include "parent.h"
#include "MPRNG.h"
extern MPRNG mprng;

void Parent::main(){
  prt.print(Printer::Parent, 'S');
  for (;;) {
    _Accept(~Parent) break;
    _Else {
      yield(parentalDelay);
      unsigned int amount = mprng(1,3);
      unsigned int recipient = mprng(numStudents - 1);
      bank.deposit(recipient, amount);
      prt.print(Printer::Parent, 'D', recipient, amount);
    }
  }
  prt.print(Printer::Parent, 'F');
}
  
Parent::Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay ) : prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}
