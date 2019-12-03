#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
    accounts = new unsigned int[numStudents];
    // Each student’s account initially starts with a balance of $0.
    for (unsigned int i = 0; i < numStudents; i ++) {
        accounts[i] = 0;
    }
    withdrawCondition = new uCondition[numStudents];
}

// The parent calls deposit to endow gifts to a specific student.
void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[id] += amount;
    withdrawCondition[id].signal();
}

// A courier calls withdraw to transfer money on behalf of the WATCard office for a specific student. The
// courier waits until enough money has been deposited, which may require multiple deposits.
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
