#include "groupoff.h"
#include "watcard.h"
#include "MPRNG.h"
extern MPRNG mprng;

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ):
    prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {
      giftCards = new WATCard::FWATCard[numStudents];
      fulfilled = new WATCard*[numStudents];
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

  for (unsigned int i = 0; i < numStudents; i ++) {
    try {
      _Accept(~Groupoff) {
        break;
      }
      _Else {
        yield(groupoffDelay);
        int assignCard = mprng(waiting - 1);
        prt.print(Printer::Groupoff, 'D', sodaCost);
        fulfilled[i] = new WATCard();
        fulfilled[i] -> deposit(sodaCost);
        giftCards[studentIndex[assignCard]].delivery(fulfilled[i]);
        std::swap(studentIndex[assignCard], studentIndex[waiting-1]);
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
  for (unsigned int i = 0 ; i < numStudents; i ++) {
    delete fulfilled[i];
  }
  delete [] fulfilled;
  delete [] giftCards;
  delete [] studentIndex;
}