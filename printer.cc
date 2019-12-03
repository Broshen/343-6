#include "printer.h"

using namespace std;

#include <iostream>

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ):
numStudents(numStudents), numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
    states = new Printer::State[numStudents + numVendingMachines + numCouriers + 6];
    
    // print header - e.g:
    // Parent Gropoff WATOff Names Truck Plant Stud0 Stud1 Mach0 Mach1 Mach2 Cour0
    // ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* ******* *******
    
    cout<<"Parent\tGropoff\tWATOff\tNames\tTruck\tPlant";
    
    
    for(unsigned int i=0; i<numStudents; i++){
        cout<<"\tStud"<<i;
    }
    
    for(unsigned int i=0; i<numVendingMachines; i++){
        cout<<"\tMach"<<i;
    }
    
    for(unsigned int i=0; i<numCouriers; i++){
        cout<<"\tCour"<<i;
    }
    
    cout<<endl;
    cout<<"*******\t*******\t*******\t*******\t*******\t*******";
    for(unsigned int i=0; i<numStudents + numCouriers + numVendingMachines; i++){
        cout<<"\t*******";
    }
    cout<<endl;
}

Printer::~Printer(){
	// print the last line that wasn't flushed
	flush(); 
	// print footer
	cout<<"***********************"<<endl;
	// free all memory allocated
	delete [] states;
}

// flush functions for different object types
// what is printed depends on the object type + state
void Printer::flush_parent(){
    switch(states[0].state){
        case 'S':
        case 'F':
            cout<<states[0].state;
            break;
        case 'D':
            cout<<states[0].state<<states[0].value1<<","<<states[0].value2;
            break;
        default:
            throw "unknown state for parent "+states[0].state;
    }
}

void Printer::flush_groupoff(){
    switch(states[1].state){
        case 'S':
        case 'F':
            cout<<states[1].state;
            break;
        case 'D':
            cout<<states[1].state<<states[1].value1;
            break;
        default:
            throw "unknown state for groupoff "+states[1].state;
    }
}

void Printer::flush_WATCard(){
    switch(states[2].state){
        case 'S':
        case 'F':
        case 'W':
            cout<<states[2].state;
            break;
        case 'C':
        case 'T':
            cout<<states[2].state<<states[2].value1<<","<<states[2].value2;
            break;
        default:
            throw "unknown state for watcard "+states[2].state;
    }
}

void Printer::flush_name(){
    switch(states[3].state){
        case 'S':
        case 'F':
            cout<<states[3].state;
            break;
        case 'R':
            cout<<states[3].state<<states[3].value1;
            break;
        case 'N':
            cout<<states[3].state<<states[3].value1<<","<<states[3].value2;
            break;
        default:
            throw "unknown state for name server "+states[3].state;
    }
}

void Printer::flush_truck(){
    switch(states[4].state){
        case 'S':
        case 'F':
            cout<<states[4].state;
            break;
        case 'P':
            cout<<states[4].state<<states[4].value1;
            break;
        case 'd':
        case 'U':
        case 'D':
            cout<<states[4].state<<states[4].value1<<","<<states[4].value2;
            break;
        default:
            throw "unknown state for truck "+states[4].state;
    }
}
void Printer::flush_bottling(){
    switch(states[5].state){
        case 'S':
        case 'F':
        case 'P':
            cout<<states[5].state;
            break;
        case 'G':
            cout<<states[5].state<<states[5].value1;
            break;
        default:
            throw "unknown state for bottling "+states[5].state;
    }
    
}
void Printer::flush_student(unsigned int id){
    switch(states[id].state){
        case 'F':
        case 'L':
            cout<<states[id].state;
            break;
        case 'V':
            cout<<states[id].state<<states[id].value1;
            break;
        case 'S':
        case 'G':
        case 'a':
        case 'B':
        case 'A':
            cout<<states[id].state<<states[id].value1<<","<<states[id].value2;
            break;
        default:
            throw "unknown state for student "+states[id].state;
    }
}
void Printer::flush_vending(unsigned int id){
    switch(states[id].state){
        case 'r':
        case 'R':
        case 'F':
            cout<<states[id].state;
            break;
        case 'S':
            cout<<states[id].state<<states[id].value1;
            break;
        case 'B':
            cout<<states[id].state<<states[id].value1<<","<<states[id].value2;
            break;
        default:
            throw "unknown state for vending "+states[id].state;
    }
}
void Printer::flush_courier(unsigned int id){
    switch(states[id].state){
        case 'S':
        case 'F':
            cout<<states[id].state;
            break;
        case 'L':
            cout<<states[id].state<<states[id].value1;
            break;
        case 't':
        case 'T':
            cout<<states[id].state<<states[id].value1<<","<<states[id].value2;
            break;
        default:
            throw "unknown state for courier "+states[id].state;
    }
}

// common function to flush a member based on its index in the states list
void Printer::flush_member(unsigned int i){
    switch(i){
        case 0:
            flush_parent();
            return;
        case 1:
            flush_groupoff();
            return;
        case 2:
            flush_WATCard();
            return;
        case 3:
            flush_name();
            return;
        case 4:
            flush_truck();
            return;
        case 5:
            flush_bottling();
            return;
    }
    
    if(i < 6 + numStudents){
        flush_student(i);
    } else if (i < 6 + numStudents + numVendingMachines){
        flush_vending(i);
    } else if (i < 6 + numStudents + numVendingMachines + numCouriers){
        flush_courier(i);
    } else{
        throw "unknown member " + i;
    }
    return;
}

void Printer::flush(){
    // only print up to the last member that has a value set
    // when this flush() is called
    for(unsigned int i=0; i<lastSet+1; i++){
        if(states[i].set){
            flush_member(i);
        }
        if(i < lastSet) cout<<"\t";
        states[i].set = false; // reset/unset all members
    }
    cout<<endl;
    lastSet = 0; // reset the lastSet tracker
}

// convert an object type to an index in the states list
unsigned int Printer::kindToIndex( Printer::Kind kind, unsigned int lid){
    switch( kind ){
        case Printer::Parent:
            return 0;
        case Printer::Groupoff:
            return 1;
        case Printer::WATCardOffice:
            return 2;
        case Printer::NameServer:
            return 3;
        case Printer::Truck:
            return 4;
        case Printer::BottlingPlant:
            return 5;
        case Printer::Student:
            return 6 + lid;
        case Printer::Vending:
            return 6 + numStudents + lid;
        case Printer::Courier:
            return 6 + numStudents + numVendingMachines + lid;
    };
    
    throw "Unknown kind" + kind;
}

// print functions with various signatures
void Printer::print( Printer::Kind kind, char state ){
    unsigned int id = kindToIndex(kind);
    if(states[id].set){
        flush();
    }
    lastSet = max(lastSet, id);
    states[id].set = true;
    states[id].state = state;
}

void Printer::print( Printer::Kind kind, char state, unsigned int value1 ){
    unsigned int id = kindToIndex(kind);
    if(states[id].set){
        flush();
    }
    lastSet = max(lastSet, id);
    states[id].set = true;
    states[id].state = state;
    states[id].value1 = value1;
    
}

void Printer::print( Printer::Kind kind, char state, unsigned int value1, unsigned int value2 ){
    unsigned int id = kindToIndex(kind);
    if(states[id].set){
        flush();
    }
    lastSet = max(lastSet, id);
    states[id].set = true;
    states[id].state = state;
    states[id].value1 = value1;
    states[id].value2 = value2;
    
}

void Printer::print( Printer::Kind kind, unsigned int lid, char state ){
    unsigned int id = kindToIndex(kind, lid);
    if(states[id].set){
        flush();
    }
    lastSet = max(lastSet, id);
    states[id].set = true;
    states[id].state = state;
}

void Printer::print( Printer::Kind kind, unsigned int lid, char state, unsigned int value1 ){
    unsigned int id = kindToIndex(kind, lid);
    if(states[id].set){
        flush();
    }
    lastSet = max(lastSet, id);
    states[id].set = true;
    states[id].state = state;
    states[id].value1 = value1;
}

void Printer::print( Printer::Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 ){
    unsigned int id = kindToIndex(kind, lid);
    if(states[id].set){
        flush();
    }
    lastSet = max(lastSet, id);
    states[id].set = true;
    states[id].state = state;
    states[id].value1 = value1;
    states[id].value2 = value2;
}

