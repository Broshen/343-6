#ifndef __PRINTER__
#define __PRINTER__


_Monitor Printer {
	struct State{
		char state;
		unsigned int value1;
		unsigned int value2;
		bool set = false;
	};

	unsigned int numStudents, numVendingMachines, numCouriers;
	unsigned int lastSet=0; // keep track of the last state in the list that was set/accessed by print()
	State *states;

	// helper flush functions for different members
	void flush_parent();
	void flush_groupoff();
	void flush_WATCard();
	void flush_name();
	void flush_truck();
	void flush_bottling();
	void flush_student(unsigned int id);
	void flush_vending(unsigned int id);
	void flush_courier(unsigned int id);
	
	// general flush function that routes to specific helpers
	void flush_member(unsigned int i);
	// general flush function to flush all members
	void flush();

  public:
	enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
	Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
	~Printer();
	void print( Kind kind, char state );
	void print( Kind kind, char state, unsigned int value1 );
	void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
	void print( Kind kind, unsigned int lid, char state );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
	void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );

  private:
  	unsigned int kindToIndex( Kind kind, unsigned int lid=0 );
};

#endif