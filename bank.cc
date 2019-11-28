#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
	accounts = new int[numStudents];
	for (unsigned int i = 0; i < numStudents; i ++) {
		accounts[i] = 0;
	}
	withdrawCondition = new uCondition[numStudents];
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
	accounts[id] += amount;
	withdrawCondition[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
	while (accounts[id] < amount) {
		withdrawCondition[id].wait();
	}

	accounts[id] -= amount;
}

Bank::~Bank() {
	delete [] accounts;
	delete [] withdrawCondition;
}