#include <iostream>

#include "BloomFilter.h"


int 
main( int argc, char* argv[] ) {

    BloomFilter<1000> bf( 500 );

    bf.addElement( 123 );

    for ( int i = 0; i < 1000000; ++i ) {
        if ( bf.containsElement( i ) ) {
            std::cout << "match -> " << i << std::endl;
        }
    }

    return 0;
}
