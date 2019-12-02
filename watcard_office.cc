#include "watcard_office.h"
#include "MPRNG.h"
extern MPRNG mprng;

// Initially, the WATCard office creates a fixed-sized courier pool with numCouriers courier tasks to
// communicate with the bank. (Additional couriers may not be created after the WATCardOffice begins.)
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): prt(prt), bank(bank), numCouriers(numCouriers) {
	courierPool = new Courier*[numCouriers];
	for (unsigned int i = 0; i < numCouriers; i ++) {
		courierPool[i] = new Courier (*this, bank, prt);
	}
}

WATCardOffice::~WATCardOffice() {
	for (unsigned int i = 0; i < numCouriers; i ++) {
		courierPool[i]->done = true;
	}
	for (unsigned int i = 0; i < numCouriers; i ++) {
		jobReady.signalBlock();
	}
	for (unsigned int i = 0; i < numCouriers; i ++) {
		delete courierPool[i];
	}
	delete [] courierPool;
}

void WATCardOffice::main() {
	prt.print(Printer::WATCardOffice, 'S');
	for (;;) {
		try {
			_Accept(~WATCardOffice) break;
			or _Accept(create, transfer, requestWork){};
		} catch (uMutexFailure::RendezvousFailure &) {}
		
		// TODO: try this _When statement to see if we can remove uCondition
		// Concern: is WatCardOffice allowed to block?
 		// or _When (!pendingJobs.empty()) _Accept(requestWork) ;
	}
	prt.print(Printer::WATCardOffice, 'F');
}

// A student performs an asynchronous call to create to create a “real” WATCard with an initial balance.
// A future WATCard is returned and sufficient funds are subsequently obtained from the bank (see Parent task) via
// a courier to satisfy the create request.
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
	// be careful of deadlock
	Job *job = new WATCardOffice::Job({sid, amount, new WATCard(), true});
	pendingJobs.push(job);
	jobReady.signal();
	prt.print(Printer::WATCardOffice, 'C', sid, amount);
	return job -> result;
} 


// A student performs an asynchronous call to transfer when its WATCard indicates there is insufficient funds
// to buy a soda. A future WATCard is returned and sufficient funds are subsequently obtained from the bank
// (see Parent task) via a courier to satisfy the transfer request.
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
	Job *job = new WATCardOffice::Job({sid, amount, card, false});
	pendingJobs.push(job);
	jobReady.signal();
	prt.print(Printer::WATCardOffice, 'T', sid, amount);
	return job -> result;
}



WATCardOffice::Job * WATCardOffice::requestWork() {
	if (pendingJobs.empty()) {
		jobReady.wait();
	}
	Job *nextJob = pendingJobs.front();
	pendingJobs.pop();
	prt.print(Printer::WATCardOffice, 'W');
	return nextJob;
}

// Each courier task calls requestWork, blocks until a Job request is ready, and then receives
// the next Job request as the result of the call. As soon as the request is satisfied (i.e., money is obtained from the
// bank), the courier updates the student’s WATCard. There is a 1 in 6 chance a courier loses a student’s WATCard
// after the update. When the card is lost, the exception WATCardOffice::Lost is inserted into the future, rather than
// making the future available, and the current WATCard is deleted.
void WATCardOffice::Courier::main() {
	prt.print(Printer::Courier, 'S');
	for (;;) {
		_Accept(~Courier){
			break;
		} _Else{
			Job *job = office.requestWork();

			if(done){
				break;
			}
			bank.withdraw(job -> args.sid, job -> args.amount);

			job -> args.watcard -> deposit(job -> args.amount);
			if (mprng(5) == 0) {
				prt.print(Printer::Courier, 'L', job->args.sid);
				job -> result.exception(new Lost());
				delete job -> args.watcard;
			} else {
				prt.print(Printer::Courier, 't', job -> args.sid, job -> args.amount);
				job -> result.delivery(job -> args.watcard);
				prt.print(Printer::Courier, 'T', job -> args.sid, job -> args.amount);
			}
			delete job;
		}
	}

	prt.print(Printer::Courier, 'F');
}