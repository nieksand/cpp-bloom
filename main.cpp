/**
 * \file demo.cpp
 *
 * Bloom filter demo program.
 *
 * This code is provided free of charge and without restrictions.  It may be
 * used for commercial products. It may be redistributed. It may be modified.
 * IT COMES WITH NO WARRANTY OF ANY SORT.
 *
 */
#include <iostream>

#include "PrimitiveBloomFilter.h"


/** 
 * Test add and contain element.
 * 
 * \return   Whether test was passed.
 */
bool
testAddContainElement( void ) {

    PrimitiveBloomFilter<double,1000> bloomFilter( 100 );

    // empty bloom filter should not contain anything
    if ( bloomFilter.containsElement( 12.1 ) ||
         bloomFilter.containsElement( 0 ) ||
         bloomFilter.containsElement( -1 ) ) {
        return false;
    }

    // add an element and verify it exists
    bloomFilter.addElement( 28 );
    if ( !bloomFilter.containsElement( 28 ) ) {
        return false;
    }

    return true;
}


/** 
 * Test hash count.
 *
 * Expected values come from:
 *   http://pages.cs.wisc.edu/~cao/papers/summary-cache/node8.html
 * 
 * \return   Whether test was passed.
 */
bool
testGetHashCount( void ) {

    // five bits per element
    PrimitiveBloomFilter<char,2500> bfBits5( 500 );
    if ( bfBits5.getHashCount() != 3 ) {
        return false;
    }

    // ten bits per element
    PrimitiveBloomFilter<char,500> bfBits10( 100 );
    if ( bfBits5.getHashCount() != 6 ) {
        return false;
    }

    // twelve bits per element
    PrimitiveBloomFilter<long,3600> bfBits12( 300 );
    if ( bfBits5.getHashCount() != 8 ) {
        return false;
    }

    // user override for hash count
    PrimitiveBloomFilter<long,3600> bfBitsCustom( 300, 3 );
    if ( bfBitsCustom.getHashCount() != 3 ) {
        return false;
    }

    return true;
}


/** 
 * Test filter union.
 * 
 * \return   Whether test was passed.
 */
bool
testUnionWith( void ) {
    return false;
}


/** 
 * Test filter intersection.
 * 
 * \return   Whether test was passed.
 */
bool
testIntersectWith( void ) {
    return false;
}



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

    int testFailureCnt = 0;

    // ensure we can add and check for elements
    if ( !testAddContainElement() ) {
        std::cerr << "\nERROR: add/contain test failed" << std::endl;
        ++testFailureCnt;
    }

    // verify number of hashes being used
    if ( !testGetHashCount() ) {
        std::cerr << "\nERROR: get hash count test failed" << std::endl;
        ++testFailureCnt;
    }


    // test results
    if ( testFailureCnt == 0 ) {
        std::cout << "\nAll tests passed\n" << std::endl;
    }
    else {
        std::cout << "\nOne or more tests failed\n" << std::endl;
    }

    return 0;
}
