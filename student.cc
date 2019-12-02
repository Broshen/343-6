#include "student.h"
#include "watcard_office.h"
#include "MPRNG.h"
extern MPRNG mprng;
Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
                 unsigned int id, unsigned int maxPurchases ) : prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}


void Student::main() {
    unsigned int toPurchase = mprng(1, maxPurchases);
    unsigned int favFlavour = mprng(3);
    prt.print(Printer::Student, id, 'S', favFlavour, toPurchase);
    
    WATCard::FWATCard watcard = cardOffice.create(id, 5);
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine *machine = nameServer.getMachine(id);
    prt.print(Printer::Student, id, 'V', machine -> getId());
    
    while(toPurchase > 0){
        yield(mprng(1, 10));
        for (;;) {
            try {
                _Select(giftCard) {
                    try {
                        machine -> buy((VendingMachine::Flavours)favFlavour, *giftCard());
                        prt.print(Printer::Student, id, 'G', favFlavour, giftCard() -> getBalance());
                        toPurchase--;
                        giftCard.reset();
                        break;
                    } catch (VendingMachine::Free &) {
                        yield(4);
                        prt.print(Printer::Student, id, 'a', favFlavour, giftCard() -> getBalance());
                        break;
                    } catch (VendingMachine::Stock &) {
                        machine = nameServer.getMachine(id);
                        prt.print(Printer::Student, id, 'V', machine -> getId());
                    }
                } or _Select(watcard) {
                    try {
                        machine -> buy((VendingMachine::Flavours)favFlavour, *watcard());
                        prt.print(Printer::Student, id, 'B', favFlavour, watcard() -> getBalance());
                        toPurchase--;
                        break;
                    } catch (VendingMachine::Free &) {
                        yield(4);
                        prt.print(Printer::Student, id, 'A', favFlavour, watcard() -> getBalance());
                        break;
                    } catch (VendingMachine::Stock &) {
                        machine = nameServer.getMachine(id);
                        prt.print(Printer::Student, id, 'V', machine -> getId());
                    } catch (VendingMachine::Funds &) {
                        watcard = cardOffice.transfer(id, machine -> cost() + 5, watcard());
												break;
                    }
                }
            } catch (WATCardOffice::Lost &) {
                // TODO: a courier can lose a student's watcard during the transfer for a new watcard so this issue can occur repeatedly
                prt.print(Printer::Student, id, 'L');
								watcard.reset();
                watcard = cardOffice.create(id, 5);
                // do not break so that i do not have to yield
            }
        }
    }

		_Select (watcard) delete watcard();

    prt.print(Printer::Student, id, 'F');
}
