Overview
--------
This is a standard bloom filter implemented in C++.

It uses Google's CityHash algorithm together with a double hashing technique for
speed and accuracy.


Contact
-------
The author, Niek Sanders, can be reached at njs@sananalytics.com.

Feedback is greatly appreciated.


Compilation
-----------
This bloom filter uses Google's CityHash code, which can be downloaded here:

  http://code.google.com/p/cityhash/

Simply update the -I and -L paths in the compile script to point at the CityHash
libraries and headers.  Running the compile script will generate a demo
executable.


Usage
-----
Coming soon.


Thread Safety
-------------
Coming soon.


Correctness
-----------
The included demo program performs a series of unit tests.

In addition to unit testing, the bloom filter false-positives counts have been
compared against the theoretical expected behavior:

![Bloom Filter False Positives](https://github.com/downloads/nieksand/cpp-bloom/bloom-error-performance.png)

The solid lines show the expected theoretical false positive rate.  The dot
marks show the actual performance of this bloom filter implementation.   The
close overlap indicates that this code is operating as expected.  This results
can be reproduced using the code in the "analysis" subdirectory.  See the readme
file in that directory for additional details.


License
-------
This bloom filter code is provided free of charge and without restrictions.  It
may be used for commercial products.  It may be redistributed.  It may be
modified.  IT COMES WITH NO WARRANTY OF ANY SORT.

If you use this code, attribution and acknowledgements are appreciated but not
required.  Buying the author a beer is, likewise, appreciated but not required.
