#ifndef __BANK__
#define __BANK__

_Monitor Bank {
	unsigned int numStudents;
	int *accounts;
  public:
	Bank( unsigned int numStudents );
	~Bank();
	void deposit( unsigned int id, unsigned int amount );
	void withdraw( unsigned int id, unsigned int amount );
};

#endif