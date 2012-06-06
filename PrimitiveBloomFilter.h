/**
 * \file PrimitiveBloomFilter.h
 *
 * BloomFilter wrapper template for primitive types.
 *
 * This code is provided free of charge and without restrictions.  It may be
 * used for commercial products. It may be redistributed. It may be modified.
 * IT COMES WITH NO WARRANTY OF ANY SORT.
 *
 */
#ifndef PRIMITIVEBLOOMFILTER_H
#define PRIMITIVEBLOOMFILTER_H

#include "BloomFilter.h"


/**
 * Bloom filter wrapper for primitive types.
 *
 * This provides a more convenient interface for adding and checking elements in
 * a bloom filter.  It will work with any class that can be properly hashed
 * using only a pointer to its raw memory region and sizeof().
 */
template< typename PrimitiveT, std::size_t NumBits >
class PrimitiveBloomFilter : public BloomFilter<NumBits> {

    public:
        /**
         * Constructor.
         *
         * \param    expectedElemCnt   Expected element count.
         * \param    numHashes         Number of hashes to use.
         */
        PrimitiveBloomFilter( std::size_t expectedElemCnt, int numHashes = -1 )
            : BloomFilter<NumBits>( expectedElemCnt, numHashes ) {
        }


        /**
         * Copy ctor.
         *
         * \param    src   Source to copy.
         */
        PrimitiveBloomFilter( const PrimitiveBloomFilter<PrimitiveT,NumBits>& src )
            : BloomFilter<NumBits>( src ) {

        }


        /**
         * Assignment operator.
         *
         * \param    rhs   Right hand side.
         *
         * \return   reference to updated self.
         */
        PrimitiveBloomFilter<PrimitiveT,NumBits> operator=(
                        const PrimitiveBloomFilter<PrimitiveT,NumBits>& rhs ) {

            BloomFilter<NumBits>::operator=( rhs );
            return *this;
        }


        /**
         * Add element to filter.
         *
         * \param    element   Element to add.
         */
        void addElement( PrimitiveT element ) {
            BloomFilter<NumBits>::addElement(
                            reinterpret_cast<char*>( &element ),
                            sizeof( PrimitiveT ) );
        }

        /**
         * Check if element is in filter.
         *
         * \param    element   Element to check.
         *
         * \return   True if element is possibly in filter; false if element is
         *           definitely not inside filter.
         */
        bool containsElement( PrimitiveT element ) const {
            return BloomFilter<NumBits>::containsElement(
                            reinterpret_cast<char*>( &element ),
                            sizeof( PrimitiveT ) );
        }

};


#endif // PRIMITIVEBLOOMFILTER_H
