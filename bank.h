#ifndef __BANK__
#define __BANK__

_Monitor Bank {
	unsigned int numStudents;
	unsigned int *accounts;
	uCondition *withdrawCondition;
  public:
	Bank( unsigned int numStudents );
	~Bank();
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

#endif