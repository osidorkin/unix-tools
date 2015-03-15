# unix-tools

Set of command line unix tools I created to speedup and simplify day-to-day work with data files. These tools provides missing features in standard unix tools like finding unique lines in unordered text stream, sorting by combination of columns, set-style operations on two text streams like union or intersection as well as significant performance improvement. Implementation heavily uses STL C++ containers like std::set and std::map.

* **uniq2**: uniq implementation which can work on unordered stream
* **setops**: set operations: union, difference, intersection, symmetric_difference
* **sortby**: fast implementation of in-memory text file sorting with ability to specify columns as a key for sort operation

## uniq2
<pre>
uniq2 filters out repeated lines in an input stream.
Unlike unix uniq it can detect non-adjucent repeated lines i.e. there is no need to sort the lines first.
As a bonus uniq2 prints lines in lexicographic order (byte representation is used for unicode characters).
Arguments:
  -c - (optional) output number of occurances for each repeated line.
  -s - (optional) swap number of occurances and line text in -c mode.
</pre>

## setops
<pre>
setops provides set operations for text files
Usage: setops [operation] &lt;[file1] 3&lt;[file2]
Available operations:
  union
  difference
  intersection
  symmetric_difference
You can use short prefix to specify operation like 'u' for 'union'.
</pre>

## sortby
<pre>
sortby is fast implementation of in-memory text file sorting with ability to specify columns as a key for sort operation
sortby reads data from standard input and writes data to standard output.
Arguments:
  -d delim - Delimiter to use to separate fields in source stream.
  -f list - Defines combination of fields to be used as a a sorting key.
             Your can use 'i' suffix to interpret field value as in integer.
  -u - Leave only first occurance for lines with same key.
  -c - In combination with -u print number of lines with the same key.
  -s - Swap number of lines and line text in -c mode.

Example: cat example.txt | sortby -d\| -f3,1i
   Will sort lines from pipe ('|') delimited file example.txt using combination of third
   and fisrt columns as a sorting key. first column value will be interpreted as an integer.

Note: if fields list is empty sortby will work as a faster version of unix sort utility.
</pre>

## benchmarks

Sorting of 1,000,000 lines text files. Each line has about 70 characters

* **sortby**: 6.251s
* unix **sort**: 31.337s
* **5x** times performance improvemnt

Finding unique lines in 1,000,000 lines text file. Each line has 16 characters. 330,000 unique lines total

* **uniq2**: 3.949s
* unix **sort+uniq**: 20.354s
* **5x** times performance improvement
