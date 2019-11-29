#include "watcard_office.h"
#include "MPRNG.h"
extern MPRNG mprng;

void WATCardOffice::Courier::main() {
	for (;;) {
		Job *job = office.requestWork();
		bank.withdraw(job ->args.sid, job -> args.amount);
		job -> args.watcard -> deposit(job -> args.amount);
		if (mprng(5) == 5) {
			job -> result.exception(new Lost());
		} else {
			job -> result.delivery(job -> args.watcard);
		}
		delete job;
	}
}

void WATCardOffice::main() {
	for (;;) {
		_Accept(~WATCardOffice) break;
		or _Accept(create, transfer);
 		or _When (!pendingJobs.empty()) _Accept(requestWork) ;
	}
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): prt(prt), bank(bank), numCouriers(numCouriers) {}
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	// be careful of deadlock
	Job *job = new WATCardOffice::Job({sid, amount, new WATCard()});
	pendingJobs.push(job);
	jobReady.signal();
	return job -> result;
} 

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
	Job *job = new WATCardOffice::Job({sid, amount, card});
	pendingJobs.push(job);
	jobReady.signal();
	return job -> result;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
	if (pendingJobs.empty()) jobReady.wait();
	Job *nextJob = pendingJobs.front();
	pendingJobs.pop();
	return nextJob;
}