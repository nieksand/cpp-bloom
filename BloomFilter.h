/**
 * \file BloomFilter.h
 *
 * BloomFilter template.
 *
 * This code is provided free of charge and without restrictions.  It may be
 * used for commercial products. It may be redistributed. It may be modified.
 * IT COMES WITH NO WARRANTY OF ANY SORT.
 *
 */
#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <algorithm>
#include <bitset>
#include <memory>
#include <stdexcept>

#include "city.h"


/**
 * Bloom filter.
 *
 * Google's CityHash algorithm is used for the hashing:
 *
 *     http://code.google.com/p/cityhash/
 *
 * This is combined with a double-hashing technique to generate the K hashes
 * needed for the bloom filter.  An explanation of this can be found here:
 *
 *     "Less hashing, same performance: Building a better bloom filter"
 *      - Kirsch and Mitzenmacher
 *
 */
template< std::size_t NumBits >
class BloomFilter {

    public:
        /**
         * Constructor.
         *
         * By default, the expected element count is used to determine the
         * number of distinct hash functions applied to each input element.
         * This can be overridden by supplying an explicit hash count.
         *
         * \param    expectedElemCnt   Expected element count.
         * \param    numHashes         Number of hashes to use.
         */
        BloomFilter( std::size_t expectedElemCnt, int numHashes = -1 );


        /**
         * Destructor.
         */
        virtual ~BloomFilter( void );


        /**
         * Copy ctor.
         *
         * \param    src   Source to copy.
         */
        BloomFilter( const BloomFilter<NumBits>& src );


        /**
         * Assignment operator.
         *
         * \param    rhs   Right hand side.
         *
         * \return   reference to updated self.
         */
        BloomFilter<NumBits>& operator=( const BloomFilter<NumBits>& rhs );


        /**
         * Add element to filter.
         *
         * \param    elementBegin   Beginning of element data.
         * \param    numBytes       Number of bytes for element.
         */
        void addElement( char* elementBegin, std::size_t numBytes );


        /**
         * Check if element is in filter.  This may return false positives, but
         * will never return false negatives.
         *
         * \param    elementBegin   Beginning of element data.
         * \param    numBytes       Number of bytes for element.
         *
         * \return   True if element is possibly inside filter; false if element
         *           is definitely not inside filter.
         */
        bool containsElement( char* elementBegin, std::size_t numBytes ) const;


        /**
         * Get hash function count.
         *
         * \return   Number of hash functions being used.
         */
        int getHashCount( void ) const;


        /**
         * Union filter contents with another bloom filter.
         *
         * \param    rhs   Right hand side.
         */
        void unionWith( const BloomFilter<NumBits>& rhs );


        /**
         * Intersection filter contents with another bloom filter.
         *
         * \param    rhs   Right hand side.
         */
        void intersectWith( const BloomFilter<NumBits>& rhs );


    private:
        /**
         * Get optimal hash function count.
         *
         * This is based on the bitvector size and expected number of elements.
         *
         * \param    expectedElemCnt   Expected element count.
         *
         * \return   optimal hash function count.
         */
        int getOptimalHashCount( std::size_t expectedElemCnt ) const;


    private:
        /**
         * Number of hash function used to map element ids to bloom bitvector.
         */
        int numHashes_;

        /**
         * Bloom filter bit vector.
         */
        std::unique_ptr< std::bitset<NumBits> > bloomBits_;
};


/*
 * Ctor
 */
template< std::size_t NumBits >
BloomFilter<NumBits>::BloomFilter( std::size_t expectedElemCnt,
                                   int numHashes )
    : numHashes_( numHashes ), bloomBits_( new std::bitset<NumBits>() ) {

    // based on bitvector size and expected element count
    if ( numHashes == -1 ) {
        numHashes_ = this->getOptimalHashCount( expectedElemCnt );
    }
}


/*
 * Dtor
 */
template< std::size_t NumBits >
BloomFilter<NumBits>::~BloomFilter( void ) {
    // allows for proper destruction of derived classes
}


/*
 * Copy ctor
 */
template< std::size_t NumBits >
BloomFilter<NumBits>::BloomFilter( const BloomFilter<NumBits>& src )
    : numHashes_( src.numHashes_ ), bloomBits_( 0 ) {

    // deep copy bitvector
    bloomBits_.reset( new std::bitset<NumBits>( *(src.bloomBits_) ) );
}


/*
 * Assignment op
 */
template< std::size_t NumBits >
BloomFilter<NumBits>&
BloomFilter<NumBits>::operator=( const BloomFilter<NumBits>& rhs ) {

    // self assignment check
    if ( &rhs == this ) {
        return *this;
    }

    numHashes_ = rhs.numHashes_;
    bloomBits_.reset( new std::bitset<NumBits>( *(rhs.bloomBits_) ) );

    return *this;
}


/*
 * Add element
 */
template< std::size_t NumBits >
void
BloomFilter<NumBits>::addElement( char* elementBegin, std::size_t numBytes ) {

    // Google CityHash
    uint128 hash = CityHash128( elementBegin, numBytes );

    // double-hashing to simulate k independent hashes
    for ( int k = 0; k < numHashes_; ++k ) {

        // bit for k-th hash
        std::size_t curBit = (hash.first + k*hash.second) % NumBits;
        bloomBits_->set( curBit );
    }
}


/*
 * Contains element
 */
template< std::size_t NumBits >
bool
BloomFilter<NumBits>::containsElement( char* elementBegin,
                                       std::size_t numBytes ) const {

    // Google CityHash for double hashing
    uint128 hash = CityHash128( elementBegin, numBytes );

    // unset bits mean definitely not in set
    for ( int k = 0; k < numHashes_; ++k ) {

        // bit for k-th hash
        std::size_t curBit = (hash.first + k*hash.second) % NumBits;
        if ( !bloomBits_->test( curBit ) ) {
            return false;
        }
    }

    // possibly in set
    return true;
}


/*
 * Get hash count
 */
template< std::size_t NumBits >
int
BloomFilter<NumBits>::getHashCount( void ) const {
    return numHashes_;
}


/*
 * Union with filters
 */
template< std::size_t NumBits >
void
BloomFilter<NumBits>::unionWith( const BloomFilter<NumBits>& rhs ) {

    // sanity check
    if ( this->getHashCount() != rhs.getHashCount() ) {
        throw std::invalid_argument( 
                    "Union for bloom filters with different hash counts" );
    }

    *(this->bloomBits_) |= *(rhs.bloomBits_);
}


/*
 * Intersect against filter
 */
template< std::size_t NumBits >
void
BloomFilter<NumBits>::intersectWith( const BloomFilter<NumBits>& rhs ) {

    // sanity check
    if ( this->getHashCount() != rhs.getHashCount() ) {
        throw std::invalid_argument( 
                    "Union for bloom filters with different hash counts" );
    }

    *(this->bloomBits_) &= *(rhs.bloomBits_);
}


/*
 * Optimal hash count
 */
template< std::size_t NumBits >
int
BloomFilter<NumBits>::getOptimalHashCount( std::size_t expectedElemCnt ) const {

    // corner case
    if ( expectedElemCnt == 0 ) {
        return 1;
    }

    // floor[(m/n) * ln(2)]
    double bitsPerElement = static_cast<double>(NumBits) / expectedElemCnt;
    int optimalK = static_cast<int>( bitsPerElement * 0.693147181 );

    // ensure at least one hash
    return std::max( optimalK, 1 );
}


#endif // BLOOMFILTER_H
