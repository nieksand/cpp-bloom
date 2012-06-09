Overview
--------
This is a standard bloom filter implemented in C++.

It uses Google's CityHash algorithm together with a double hashing technique for
speed and accuracy.

Background on the double hashing technique can be found in "Less hashing,
same performance: Building a better bloom filter" by Kirsch and Mitzenmacher.
(http://dx.doi.org/10.1002/rsa.v33:2)


Contact
-------
The author, Niek Sanders, can be reached at njs@sananalytics.com.

Feedback is greatly appreciated.


Compilation
-----------
This bloom filter uses Google's CityHash code, which can be downloaded here:

  http://code.google.com/p/cityhash/

After installing CityHash, simply update the -I and -L paths in the compile 
script to point at the CityHash headers and libraries.  Running the compile 
script will generate a demo executable.


Usage
-----
See the included demo program for basic usage examples.

A bloom filter has two fundamental operations: addElement() and
containsElement().

  * addElement() operation does what the name implies.

  * containsElement() query may give false positives about set membership, but
    it may never give false negatives.

There are also routines for computing union and intersection between filters.
The union operation is useful when combining results from a distributed
computation.  Each thread or process computes filters for its portion of the
input.  These partial results are unioned together to give a filter for the
complete set.

The included PrimitiveBloomFilter template is simply a wrapper around
BloomFilter that provides a nicer programming interface.


Thread Safety
-------------
This code will be reentrant if your C++ STL implementation is also reentrant.  

The default distribution is not thread safe, but can be made so via fairly
trivial changes.


Correctness
-----------
The included demo program performs a series of unit tests.

In addition to unit testing, the bloom filter false-positives counts have been
compared against the theoretical expected behavior:

![Bloom Filter False Positives](https://github.com/downloads/nieksand/cpp-bloom/bloom-error-performance.png)

The solid lines show the expected theoretical false positive rate.  The dot
marks show the actual performance of this bloom filter implementation.   The
close overlap indicates that this code is operating as expected.  These results
can be reproduced using the code in the "analysis" subdirectory.  See the readme
file in that directory for additional details.


License
-------
This bloom filter code is provided free of charge and without restrictions.  It
may be used for commercial products.  It may be redistributed.  It may be
modified.  IT COMES WITH NO WARRANTY OF ANY SORT.

If you use this code, attribution and acknowledgements are appreciated but not
required.  Buying the author a beer is, likewise, appreciated but not required.
