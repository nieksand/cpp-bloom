#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include <bitset>
#include <memory>

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
 *     "XYZ PAPER"
 *
 */
template< std::size_t NumBits >
class BloomFilter {

    public:
        /**
         * Constructor.
         *
         * The expected element count is used to determine the number of
         * distinct hash functions applied to each input element.
         *
         * \param    expectedElemCnt   Expected element count.
         */
        BloomFilter( std::size_t expectedElemCnt );


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
         * \param    elementId   Element id.
         */
        void addElement( long elementId );

        /**
         * Check if element is in filter.  This may return false positives, but
         * will never return false negatives.
         *
         * \param    elementId   Element id.
         *
         * \return   True if element is possibly inside filter; false if element
         *           is definitely not inside filter.
         */
        bool containsElement( long elementId ) const;

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
BloomFilter<NumBits>::BloomFilter( std::size_t expectedElemCnt )
    : numHashes_( -1 ), bloomBits_( new std::bitset<NumBits>() ) {

    // based on bitvector size and expected element count
    numHashes_ = this->getOptimalHashCount( expectedElemCnt );
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
BloomFilter<NumBits>::addElement( long elementId ) {

    // Google CityHash
    char* elemPtr = reinterpret_cast<char*>( &elementId );
    uint128 hash = CityHash128( elemPtr, sizeof(elementId) );

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
BloomFilter<NumBits>::containsElement( long elementId ) const {

    // Google CityHash for double hashing
    char* elemPtr = reinterpret_cast<char*>( &elementId );
    uint128 hash = CityHash128( elemPtr, sizeof(elementId) );

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
 * Union with filters
 */
template< std::size_t NumBits >
void
BloomFilter<NumBits>::unionWith( const BloomFilter<NumBits>& rhs ) {
    *(this->bloomBits_) |= rhs.bloomBits_;
}


/*
 * Intersect against filter
 */
template< std::size_t NumBits >
void
BloomFilter<NumBits>::intersectWith( const BloomFilter<NumBits>& rhs ) {
    *(this->bloomBits_) &= rhs.bloomBits_;
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

    // ceil[(m/n) * ln(2)]
    return static_cast<int>( NumBits / expectedElemCnt * 0.693147181 + 0.5 );
}


#endif // BLOOMFILTER_H
