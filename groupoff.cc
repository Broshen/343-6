#include "groupoff.h"
#include "watcard.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
    prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
      giftCards = new WATCard::FWATCard[numStudents];
    }

void Groupoff::main(){
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
        //TODO: randomness
        // assignCard = MPRNG(i, numStudents);
        fulfilledCards[i].deposit(sodaCost);
        giftCards[i].delivery(&fulfilledCards[i]);

      }
		} catch (uMutexFailure::RendezvousFailure &) {}
		
  }
}
WATCard::FWATCard Groupoff::giftCard(){
  return giftCards[waiting++];
}

Groupoff::~Groupoff() {
  delete [] giftCards;
}