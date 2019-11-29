#include "watcard_office.h"
#include "MPRNG.h"
extern MPRNG mprng;

void WATCardOffice::Courier::main() {
	for (;;) {
		Job *job = office.requestWork();
		prt.print(Printer::WATCardOffice, 'W');
		bank.withdraw(job -> args.sid, job -> args.amount);
		job -> args.watcard -> deposit(job -> args.amount);
		if (mprng(5) == 0) {
			job -> result.exception(new Lost());
			delete job -> args.watcard;
		} else {
			job -> result.delivery(job -> args.watcard);
		}
		if (job -> args.isCreate) {
			prt.print(Printer::WATCardOffice, 'C', job -> args.sid, job -> args.amount);
		} else {
			prt.print(Printer::WATCardOffice, 'T', job -> args.sid, job -> args.amount);
		}
		delete job;
	}
}

void WATCardOffice::main() {
	prt.print(Printer::WATCardOffice, 'S');
	for (;;) {
		_Accept(~WATCardOffice) break;
		or _Accept(create, transfer);
		// TODO: try this _When statement to see if we can remove uCondition
		// Concern: is WatCardOffice allowed to block?
 		// or _When (!pendingJobs.empty()) _Accept(requestWork) ;
	}
	prt.print(Printer::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): prt(prt), bank(bank), numCouriers(numCouriers) {}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	// be careful of deadlock
	Job *job = new WATCardOffice::Job({sid, amount, new WATCard(), true});
	pendingJobs.push(job);
	jobReady.signal();
	return job -> result;
} 

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
	Job *job = new WATCardOffice::Job({sid, amount, card, false});
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