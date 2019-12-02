#ifndef __WATCARD_OFFICE_H__
#define __WATCARD_OFFICE_H__

#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include <queue>
using namespace std;

_Task WATCardOffice {
	struct Job {							// marshalled arguments and return future
		struct Args {
			unsigned int sid, amount;						// call arguments (YOU DEFINE "Args")
			WATCard *watcard;
			bool isCreate;
		};
		Args args;
		WATCard::FWATCard result;			// return future
		Job( Args args ) : args(args) {}
	};

	_Task Courier { 
		WATCardOffice &office;
		Bank &bank;
		Printer &prt;
		void main();
	public:
		bool done=false;
		Courier(WATCardOffice &office, Bank &bank, Printer &prt): office(office), bank(bank), prt(prt) {}
	};					// communicates with bank

	Printer &prt;
	Bank &bank;
	unsigned int numCouriers;
	uCondition jobReady;
	queue<Job *> pendingJobs;
	Courier **courierPool;
	void main();
  public:
	_Event Lost {};							// lost WATCard
	WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
	WATCard::FWATCard create( unsigned int sid, unsigned int amount );
	WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
	Job * requestWork();
	~WATCardOffice();
};

#endif