#include <vector>
#include <iostream>
#include <cmath>


int 
main( int argc, char* argv[] ) {

    
    // filter size
    double m = 10000;

    // element counts
    std::vector<double> expectedElems;
    for ( int bitsPerElem = 3; bitsPerElem <= 12; bitsPerElem += 3 ) {
        expectedElems.push_back( m / bitsPerElem );
    }

    // actual element insertions
    for ( int i = 1; i <= 80000; ++i ) {

        std::cout << i << ' ';

        // under each element count scenario
        for ( std::size_t j = 0; j < expectedElems.size(); ++j ) {
            double n = expectedElems.at(j);
            double k = (m/n) * std::log( 2 );
            double errorRate = std::pow( (1 - std::pow( 1-(1/m), k*i )), k );
            std::cout << errorRate << ' ';
        }

        if ( i % 1000 == 0 ) {
            std::cerr << i/80000.0 << std::endl;
        }

        std::cout << '\n';
    }
    std::cout << std::flush;

    return 0;
}
