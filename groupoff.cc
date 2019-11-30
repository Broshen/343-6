#include "groupoff.h"
#include "watcard.h"
#include "MPRNG.h"
extern MPRNG mprng;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
    prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
      giftCards = new WATCard::FWATCard[numStudents];
      studentIndex = new int[numStudents];
      for (unsigned int i = 0 ; i < numStudents; i ++) {
       studentIndex[i] = i; 
      }
    }

void Groupoff::main(){
  prt.print(Printer::Groupoff, 'S');
  for (unsigned int i = 0; i < numStudents; i ++) {
    try { 
      _Accept(giftCard);
    } catch (uMutexFailure::RendezvousFailure &) {}

  }
  WATCard fulfilledCards[numStudents];
  for (unsigned int i = 0; i < numStudents; i ++) {
    try {
      _Accept(~Groupoff) break;
			_Else {
        yield(groupoffDelay);
        // TODO: ensure waiting = numstudents at start
        int assignCard = mprng(waiting - 1);
        prt.print(Printer::Groupoff, 'D', sodaCost);
        fulfilledCards[i].deposit(sodaCost);
        giftCards[assignCard].delivery(&fulfilledCards[i]);
        std::swap(studentIndex[assignCard], studentIndex[waiting]);
        waiting --;
      }
		} catch (uMutexFailure::RendezvousFailure &) {}
  }
  prt.print(Printer::Groupoff, 'F');
}
WATCard::FWATCard Groupoff::giftCard(){
  return giftCards[waiting++];
}

Groupoff::~Groupoff() {
  delete [] giftCards;
  delete [] studentIndex;
}