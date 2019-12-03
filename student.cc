#include "student.h"
#include "watcard_office.h"
#include "MPRNG.h"

extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
                 unsigned int id, unsigned int maxPurchases ) : prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}


void Student::main() {
    
    // A student begins by selecting a random number of bottles to purchase [1, MaxPurchases], a
    // random favourite flavour [0, 3], creates a WATCard via the WATCardOffice with a $5 balance, creates a gift card
    // via the Groupoff with a value of SodaCost, and obtains the location of a vending machine from the name server.
    unsigned int toPurchase = mprng(1, maxPurchases);
    unsigned int favFlavour = mprng(3);
    prt.print(Printer::Student, id, 'S', favFlavour, toPurchase);
    
    WATCard::FWATCard watcard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine *machine = nameServer.getMachine(id);
    prt.print(Printer::Student, id, 'V', machine -> getId());
    
    // A student terminates after purchasing all the soda initially selected.
    while(toPurchase > 0){
        // Before each attempt to buy a soda, a student yields a random number of times in the range [1, 10].
        yield(mprng(1, 10));
        
        for (;;) {
            try {
                // wait for money to be transferred either from Groupoff to their gift card
                // or from the WATCardOffice to their WATCard
                // Once money is available on either card, the student can attempt
                // to buy a bottle of soda from their current vending machine.
                _Select(giftCard) {
                    try {
                        machine -> buy((VendingMachine::Flavours)favFlavour, *giftCard());
                        prt.print(Printer::Student, id, 'G', favFlavour, giftCard() -> getBalance());
                        toPurchase--;
                        // giftcard future can only be used once, so it must be reset after use to prevent any further usage.
                        giftCard.reset();
                        break;
                    } catch (VendingMachine::Free &) {
                        // If the vending machine delivers a free bottle of soda (which does not count as a purchased
                        // soda), the student must watch an advertisement by yielding 4 times whiling drinking the
                        // free sode and then attempts another purchase.
                        yield(4);
                        prt.print(Printer::Student, id, 'a', favFlavour, giftCard() -> getBalance());
                        break; // break to attempt another purchase
                    } catch (VendingMachine::Stock &) {
                        // If the vending machine is out of the student’s favourite flavour, the student must obtain a new
                        // vending machine from the name server and attempt another purchase.
                        machine = nameServer.getMachine(id);
                        prt.print(Printer::Student, id, 'V', machine -> getId());
                        break;
                    }
                } or _Select(watcard) {
                    try {
                        machine -> buy((VendingMachine::Flavours)favFlavour, *watcard());
                        // If the vending machine debits the WATCard and delivers a bottle of soda, the student drinks it and attempts
                        // another purchase.
                        prt.print(Printer::Student, id, 'B', favFlavour, watcard() -> getBalance());
                        toPurchase--;
                        break; // break to attempt another purchase
                    } catch (VendingMachine::Free &) {
                        // If the vending machine delivers a free bottle of soda (which does not count as a purchased
                        // soda), the student must watch an advertisement by yielding 4 times whiling drinking the
                        // free sode and then attempts another purchase.
                        yield(4);
                        prt.print(Printer::Student, id, 'A', favFlavour, watcard() -> getBalance());
                        break; // break to attempt another purchase
                    } catch (VendingMachine::Stock &) {
                        // If the vending machine is out of the student’s favourite flavour, the student must obtain a new
                        // vending machine from the name server and attempt another purchase.
                        machine = nameServer.getMachine(id);
                        prt.print(Printer::Student, id, 'V', machine -> getId());
                        break;
                    } catch (VendingMachine::Funds &) {
                        // If the vending machine indicates insufficient funds, a student
                        // transfers the current vending-machine soda-cost plus $5 to their WATCard
                        // via the WATCard office and attempts another purchase.
                        watcard = cardOffice.transfer(id, machine -> cost() + 5, watcard());
                        break; // break to attempt another purchase
                    } catch (WATCardOffice::Lost &) {
                        // If a courier has lost a student’s WATCard during a transfer, the exception WATCardOffice::Lost
                        // is raised when the future value is accessed. In this case, the student must create a new
                        // WATCard via the WATCardOffice with a $5 balance, and re-attempt to buy a soda but without
                        // yielding as no call to buy has occurred. Note, a courier can lose a student’s WATCard during
                        // the transfer for the new WATCard so this issue can occur repeatedly.
                        prt.print(Printer::Student, id, 'L');
                        watcard.reset();
                        watcard = cardOffice.create(id, 5);
                        // do not break so that i do not have to yield
                    }
                }
            } catch (WATCardOffice::Lost &) {
                // catch clause outside of _Select statements to catch case when Lost is thrown when
                // watcard is first accessed in _Select statement
                prt.print(Printer::Student, id, 'L');
                watcard.reset();
                watcard = cardOffice.create(id, 5);
                // do not break so that i do not have to yield
            }
        }
    }
    // cleanup
    try {
        _Select (watcard) delete watcard(); 
    } catch (WATCardOffice::Lost &) {}
    
    
    prt.print(Printer::Student, id, 'F');
}
