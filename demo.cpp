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
    PrimitiveBloomFilter<char,1000> bfBits10( 100 );
    if ( bfBits10.getHashCount() != 6 ) {
        return false;
    }

    // twelve bits per element
    PrimitiveBloomFilter<long,3600> bfBits12( 300 );
    if ( bfBits12.getHashCount() != 8 ) {
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

    // two bloom filters, each with an element
    PrimitiveBloomFilter<long,10000> bfA( 1000 );
    bfA.addElement( 7 );

    PrimitiveBloomFilter<long,10000> bfB( 1000 );
    bfB.addElement( 100 );

    // union should contain both elements
    bfA.unionWith( bfB );
    if ( !bfA.containsElement( 7 ) || !bfB.containsElement( 100 ) ) {
        return false;
    }

    return true;
}


/**
 * Test filter intersection.
 *
 * \return   Whether test was passed.
 */
bool
testIntersectWith( void ) {

    // two bloom filters
    PrimitiveBloomFilter<float,5000> bfA( 1000, 3 );
    bfA.addElement( 12.3 );

    PrimitiveBloomFilter<float,5000> bfB( 1000, 3 );
    bfB.addElement( 40 );

    // ensure elements are in their own filters
    if ( !bfA.containsElement( 12.3 ) || !bfB.containsElement( 40 ) ) {
        return false;
    }

    // elements hashing to same filter mean intersection test won't work
    if ( bfA.containsElement( 40 ) || bfB.containsElement( 12.3 ) ) {
        return false;
    }

    // neither element should be in intersection
    bfA.intersectWith( bfB );
    if ( bfA.containsElement( 40 ) || bfA.containsElement( 12.3 ) ) {
        return false;
    }

    return true;
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

    // boom filter union
    if ( !testUnionWith() ) {
        std::cerr << "\nERROR: union test failed" << std::endl;
        ++testFailureCnt;
    }

    // bloom filter intersection
    if ( !testIntersectWith() ) {
        std::cerr << "\nERROR: intersection test failed" << std::endl;
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
