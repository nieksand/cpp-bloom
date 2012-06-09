Overview
--------
This code calculates the actual false positive rate of the cpp-bloom filter
implementation.

The data can be visualized against the theoretical results as follows:

    ./compile
    ./error_rate
    gnuplot < make_plot

GnuPlot will generate a PNG graph showing the expected theoretical behavior
(solid lines) against the actual behavior (dots).
