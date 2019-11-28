#include "printer.h"

// Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier 

void testPrinter(){
	Printer printer(3,3,3);
	printer.print(Printer::Parent, 'S');
	printer.print(Printer::Parent, 'F');
	printer.print(Printer::Parent, 'D', 1, 2);
	printer.print(Printer::Groupoff, 'S');
	printer.print(Printer::Groupoff, 'F');
	printer.print(Printer::Groupoff, 'D', 10);
	printer.print(Printer::WATCardOffice, 'S');
	printer.print(Printer::WATCardOffice, 'F');
	printer.print(Printer::WATCardOffice, 'W');
	printer.print(Printer::WATCardOffice, 'C',11,20);
	printer.print(Printer::WATCardOffice, 'T',30,40);
	printer.print(Printer::NameServer, 'S');
	printer.print(Printer::NameServer, 'F');
	printer.print(Printer::NameServer, 'R', 50);
	printer.print(Printer::NameServer, 'N', 60, 70);
	printer.print(Printer::Truck, 'S');
	printer.print(Printer::Truck, 'F');
	printer.print(Printer::Truck, 'P',100);
	printer.print(Printer::Truck, 'd',22,33);
	printer.print(Printer::Truck, 'U',4,5);
	printer.print(Printer::Truck, 'D',6,7);
	printer.print(Printer::BottlingPlant, 'S');
	printer.print(Printer::BottlingPlant, 'F');
	printer.print(Printer::BottlingPlant, 'P');
	printer.print(Printer::BottlingPlant, 'G', 8);

	for(int i=0;i<3;i++){

		printer.print(Printer::Student, i, 'F');
		printer.print(Printer::Student, i, 'L');
		printer.print(Printer::Student, i, 'V', i+9);
		printer.print(Printer::Student, i, 'S', i+10, i+11);
		printer.print(Printer::Student, i, 'G', i+12, i+13);
		printer.print(Printer::Student, i, 'a', i+14, i+15);
		printer.print(Printer::Student, i, 'B', i+16, i+17);
		printer.print(Printer::Student, i, 'A', i+18, i+19);


		printer.print(Printer::Vending, i, 'r');
		printer.print(Printer::Vending, i, 'R');
		printer.print(Printer::Vending, i, 'F');
		printer.print(Printer::Vending, i, 'S', i+20);
		printer.print(Printer::Vending, i, 'B', i+21, i+22);

		printer.print(Printer::Courier, i, 'S');
		printer.print(Printer::Courier, i, 'F');
		printer.print(Printer::Courier, i, 'L', i+23);
		printer.print(Printer::Courier, i, 't', i+24, i+25);
		printer.print(Printer::Courier, i, 'T', i+26, i+27);
	}

}