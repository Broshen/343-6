#include "student.h"
#include "watcard_office.h"
#include "MPRNG.h"
extern MPRNG mprng;
Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff,
  			 unsigned int id, unsigned int maxPurchases ) : prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), maxPurchases(maxPurchases) {}


void Student::main() {
	unsigned int toPurchase = mprng(1, maxPurchases);
	unsigned int favFlavour = mprng(3);
	WATCard::FWATCard watcard = cardOffice.create(id, 5);
	WATCard::FWATCard giftCard = groupoff.giftCard();
	// obtain location of vending machine

	for (unsigned int i; i < toPurchase; i ++) {
		yield(mprng(1, 10));
		for (;;) {
			_Select(giftCard) {
					// buy soda using giftcard
			} or _Select(watcard) {
					// buy soda using watcard
			}

		}
	}


}
