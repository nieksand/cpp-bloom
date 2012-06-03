#include <iostream>

#include "PrimitiveBloomFilter.h"


/**
 * Main program.
 *
 * \param    argc   Number of command line arguments.
 * \param    argv   Text of command line arguments.
 *
 * \return   zero if successful; nonzero otherwise.
 */
int
main( int argc, char* argv[] ) {

    PrimitiveBloomFilter<long,800> bf( 100 );

    // add in elements
    int j =0 ;
    for ( int i = 0; i < 1000; i += 10 ) {
        bf.addElement( i );
    }

    // look at both real hits and false positives
    for ( int i = 0; i < 2000; ++i ) {
        if ( bf.containsElement( i ) ) {
            std::cout << "match -> " << i << std::endl;
        }
    }

    return 0;
}
