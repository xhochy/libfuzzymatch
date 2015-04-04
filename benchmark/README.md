Benchmarks
==========

This directory contains various benchmarks to evaluate the performance of libfuzzymatch with software packages providing similar functionality.
It should serve as a comparision to the yet available performance in other libraries and that implemented in libfuzzymatch.
As we do not claim to be experts in all those libs, some of the benchmarks may be not implemented in the best possible way, if so, please do a Pull Request.

In the following, we will describe the steps taken in each benchmark and when the timing starts in them.

\#1 Creating an index from an UTF-8 input
-----------------------------------------

 1. Load a large number of UTF-8 strings into memory
 2. START timing
 3. Add each string to the index
 4. Commit the index / perform some optimisations on the index as provided by the implementation
 5. END timing
 6. Store the index on a harddrive

\#2 Querying UTF-8 terms on the previously generated index
----------------------------------------------------------

 1. Load some UTF-8 queries into memory (these string should have an edit distance between 0 and 2 to at least an entry in the index)
 2. START timing
 3. Query for the best 20 matches for each string
 4. END timing

TODO
----

These are all ideas for futher benchmarks:

 * [ ] Add non-UTF-8 benchmarks
   * [ ] ASCII
   * [ ] ISO-8859-?
   * [ ] UTF-16
   * [ ] UIF-32
 * Size of the index
 * Index loading and storing
 * Different data sets
 * Query string with edit distance > 2
