#include "watcard_office.h"
#include "MPRNG.h"
extern MPRNG mprng;

// Initially, the WATCard office creates a fixed-sized courier pool with numCouriers courier tasks to
// communicate with the bank. (Additional couriers may not be created after the WATCardOffice begins.)
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): prt(prt), bank(bank), numCouriers(numCouriers) {
	courierPool = new Courier*[numCouriers];
	for (unsigned int i = 0; i < numCouriers; i ++) {
		courierPool[i] = new Courier (i, *this, bank, prt);
	}
}

// need to delete all couriers on destructor
WATCardOffice::~WATCardOffice() {
	// set deletion flag to true for all couriers
	for (unsigned int i = 0; i < numCouriers; i ++) {
		courierPool[i]->done = true;
	}
	// wake up all couriers, so that they can see the flag, and exit their main()
	for (unsigned int i = 0; i < numCouriers; i ++) {
		jobReady.signalBlock();
	}
	// wait for courier tasks to finish and delete
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
			or _Accept(create, transfer){}
			or _Accept(requestWork) {}
		} catch (uMutexFailure::RendezvousFailure &) {}
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
	// we may be woken up by the destructor signalling shutdown & no more jobs
	// in which case, the queue would be empty. in this case, don't print anything
	// and just return a nullptr
	if (pendingJobs.empty()){
		return nullptr;
	}

	Job *nextJob = pendingJobs.front();
	pendingJobs.pop();
	prt.print(Printer::WATCardOffice, 'W');
	return nextJob;
}

void WATCardOffice::Courier::main() {
	prt.print(Printer::Courier, id, 'S');
	for (;;) {
		_Accept(~Courier){
			break;
		} _Else{

			// Each courier task calls requestWork, blocks until a Job request is ready, and then receives
			// the next Job request as the result of the call.
			Job *job = office.requestWork();

			// If courier was actually woken up to be shut down, break from loop and exit
			if(done){
				break;
			}

			prt.print(Printer::Courier, id, 't', job -> args.sid, job -> args.amount);

			// obtain money from the bank and update the student’s WATCard.
			bank.withdraw(job -> args.sid, job -> args.amount);
			job -> args.watcard -> deposit(job -> args.amount);


			// 1 in 6 chance a courier loses a student’s WATCard after the update.
			if (mprng(5) == 0) {
				// When the card is lost, the exception WATCardOffice::Lost is inserted into the future
				prt.print(Printer::Courier,  id, 'L', job->args.sid);
				job -> result.exception(new Lost());
				// current WATCard is deleted.
				delete job -> args.watcard;
			} else {
				job -> result.delivery(job -> args.watcard);
				prt.print(Printer::Courier,  id,'T', job -> args.sid, job -> args.amount);
			}
			delete job;
		}
	}

	prt.print(Printer::Courier, id, 'F');
}