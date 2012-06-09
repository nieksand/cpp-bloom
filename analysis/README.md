Overview
--------
This code verifies the correctness of the cpp-bloom filter implementation.

There are theoretical results for the false positive rate as a function of
element count and filter size in bits.  For details, see the dicussion in
http://en.wikipedia.org/wiki/Bloom_filter

We calculate the actual false positive rate from our bloom filter code and
compare against these theoretical results.  If the values match closely, then we
have a strong indicator of code correctness.

After compiling the code, the results can be visualized using GnuPlot:

    ./compile
    ./error_rate
    gnuplot < make_plot

GnuPlot will generate a PNG image showing the expected theoretical behavior
(solid lines) against the actual behavior (dots).
