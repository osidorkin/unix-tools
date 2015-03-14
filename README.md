# unix-tools

Set of command line unix tools I created to speedup and simplify day-to-day work with data files. These tools provides unique missing features in standard unix tools like finding unique lines in unordered text stream, sorting by combination of columns, set-style operations on two text streams like union or intersection as well as X times performance improvement. Implementation heavily uses STL C++ containers like std::set and std::map.

* **uniq2**: uniq implementation which can work on unordered stream
* **setops**: basic set operations: union, difference, intersection, symmetric_difference
* **sortby**: fast implementation of in-memory text file sorting with ability to specify columns as a key for sort operation

## uniq2
<pre>
description to be provided
</pre>

## setops
<pre>
description to be provided
</pre>

## sortby
<pre>
sortby is fast implementation of in-memory text file sorting with
ability to specify columns as a key for sort operation
sortby reads data from standard input and writes data to standard output
arguments:
  -d<delimiter> - column delimiter string
  -f<fields> - fields is command separated list of column 1-based numbers to use as a sorting key
               to interpret column text as an integer add 'i' suffix after column number
  -u - (optional) leave only first occurance for lines with same key
  -c - (optional) in combination with -u will print number of lines with the same key

example: cat example.txt | sortby -d\| -f3,1i
   will sort lines from pipe ('|') delimited file example.txt using combination of third
   and fisrt columns as a sorting key. first column value will be interpreted as an integer

note: if fields list is empty sortby will work as a faster version of unix sort utility
sortby is fast implementation of in-memory text file sorting with
ability to specify columns as a key for sort operation.
sortby reads data from standard input and writes data to standard output
</pre>

## benchmarks

Sorting of 1,000,000 millions lines text files. Each line has about 70 characters

* **sortby**: 6.251s
* unix **sort**: 31.337s
* **5x performance improvemnt**

Finding unique lines in 1,000,000 unordered text file. Each line has 16 characters. 330,000 unique lines

* **uniq2**: 3.949s
* unix **sort+uniq**: 20.354s
* **5x performance improvement**
