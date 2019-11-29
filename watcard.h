#ifndef __WATCARD__
#define __WATCARD__
#include <uFuture.h>

class WATCard {
	WATCard( const WATCard & ) = delete;	// prevent copying
	WATCard & operator=( const WATCard & ) = delete;
	unsigned int balance = 0;
  public:
	typedef Future_ISM<WATCard *> FWATCard;	// future watcard pointer
	WATCard();
	void deposit( unsigned int amount );
	void withdraw( unsigned int amount );
	unsigned int getBalance();
};

#endif