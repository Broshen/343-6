// Purpose: copy file
//
// Command syntax:
//   $ ./a.out [ size (>= 0) | 'd' (default 20) [ code (>= 0) | 'd' (default 5) [ input-file [ output-file ] ] ] ]
// all parameters are optional. Defaults: size=>20, code=>5, input=>cin, output=>cout
//
// Examples:
//   $ ./a.out 34
//   $ ./a.out 34 0
//   $ ./a.out 34 0 in
//   $ ./a.out 34 0 in out

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;					// direct access to std
#include <cstdlib>					// exit
#include "config.h"


bool convert( int & val, char * buffer ) {		// convert C string to integer
    std::stringstream ss( buffer );			// connect stream and buffer
    string temp;
    ss >> dec >> val;					// convert integer from buffer
    return ! ss.fail() &&				// conversion successful ?
	! ( ss >> temp );				// characters after conversion all blank ?
} // convert


int main( int argc, char * argv[] ) {
    // MUST BE INT (NOT UNSIGNED) TO CORRECTLY TEST FOR NEGATIVE VALUES
    int seed = getpid();
    string filename = "soda.config";

    try {
	switch ( argc ) {
	 case 3:
	    if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
		if ( ! convert( seed, argv[2] ) || seed <= 0 ) throw 1; // invalid ?
	    } // if
	    // FALL THROUGH
	 case 2:
	  	filename = argv[1];
	    // FALL THROUGH
	 case 1:					// defaults
	 break;
	  default:					// wrong number of options
	    throw 1;
	} // switch
    } catch( ... ) {
	cerr << "Usage: " << argv[0]
	     << " [ config-file [ Seed ] ]" << endl;
	exit( EXIT_FAILURE );				// TERMINATE
    } // try
    //cout << "size " << size << " code " << code << endl;
	ConfigParms configParms;
    processConfigFile(filename.c_str(), configParms);

} // main

// Local Variables: //
// compile-command: "u++ uIO.cc" //
// End: //