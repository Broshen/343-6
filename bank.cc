#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
	accounts = new int[numStudents];
	for (unsigned int i = 0; i < numStudents; i ++) {
		accounts[i] = 0;
	}
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
	accounts[id] += amount;
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
	while (accounts[id] < amount) {
		try {
			_Accept(deposit);
		} catch (uMutexFailure::RendezvousFailure) {}
	}

	accounts[id] -= amount;
}

Bank::~Bank() {
	delete [] accounts;
}