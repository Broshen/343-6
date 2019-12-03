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
    // The Groupoff task begins by accepting a call from each student to obtain a future gift-card.
    for (unsigned int i = 0; i < numStudents; i ++) {
        try {
            _Accept(giftCard);
        } catch (uMutexFailure::RendezvousFailure &) {}
        
    }
    
    // loop until all the future gift-cards are assigned a real WATCard or a call to
    // its destructor occurs
    for (unsigned int i = 0; i < numStudents; i ++) {
        try {
            _Accept(~Groupoff) {
                break;
            }
            _Else {
                // Before each future gift-card is assigned a real WATCard, groupoff yields for groupoffDelay
                // times (not random).
                yield(groupoffDelay);
                
                // periodically puts a real WATCard with value SodaCost into a random future gift-card.
                // A future gift-card is assigned only once.
                int assignCard = mprng(waiting - 1);
                prt.print(Printer::Groupoff, 'D', sodaCost);
                fulfilled[i] = new WATCard();
                fulfilled[i] -> deposit(sodaCost);
                giftCards[studentIndex[assignCard]].delivery(fulfilled[i]);
                
                // remove this student from the set of students waiting to receive a giftcard
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
