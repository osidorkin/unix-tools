#include <set>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <memory.h>
#include "stream.h"

using namespace std;

void help() {
    cout << 
        "sortby is fast implementation of in-memory text file sorting with\n"
        "ability to specify columns as a key for sort operation\n"
        "sortby reads data from standard input and writes data to standard output\n"
        "arguments:\n"
        "  -d[delimiter] - (optional)column delimiter string\n"
        "  -f[fields] - (optional)fields is command separated list of column 1-based numbers to use as a sorting key\n"
        "               to interpret column text as an integer add 'i' suffix after column number\n"
        "  -u - (optional) leave only first occurance for lines with same key\n"
        "  -c - (optional) in combination with -u will print number of lines with the same key\n"
        "\nexample: cat example.txt | sortby -d\\| -f3,1i\n"
        "   will sort lines from pipe ('|') delimited file example.txt using combination of third\n"
        "   and fisrt columns as a sorting key. first column value will be interpreted as an integer\n"
        "\nnote: if fields list is empty sortby will work as a faster version of unix sort utility\n";
}

inline const char* advance(const char* p, int n, const char* d, size_t l, size_t* s) {
    while (p && n-->0) {
        p = strstr(p, d);
        if (p)
            p += l;
    }
    if (!p)
        p = "";
    const char* q = strstr(p, d);
    *s = q ? q-p : strlen(p);
    return p;
}

struct compare_t {
    string delimiter;
    typedef vector< pair<int,int> > fields_t;
    fields_t fields;
    bool operator()(const char* p, const char* q) const {
        if (fields.empty())
            return strcmp(p,q) < 0;
        for (fields_t::const_iterator i=fields.begin(), e=fields.end(); i!=e; ++i) {
            size_t s1, s2;
            const char* f1 = advance(p, i->first, delimiter.c_str(), delimiter.size(), &s1);
            const char* f2 = advance(q, i->first, delimiter.c_str(), delimiter.size(), &s2);
            if (i->second == 'i') {
                long int i1 = atol(f1);
                long int i2 = atol(f2);
                if (i1 == i2) continue;
                return i1 < i2;
            }
            int c = memcmp(f1, f2, min(s1,s2));
            if (c == 0) {
                if (s1 == s2)
                    continue;
                return s1 < s2;
            }
            return c < 0;
        }
        return false;
    }
};

int main(int argc, char** argv) {

    compare_t compare;
    bool unique = false;
    bool count = false;

    for (int i=1; i<argc; ++i) {
        const char* arg = argv[i];
        if (strstr(arg, "-h") == arg || strstr(arg, "--h") == arg) {
            help();
            return 1;
        }
        else if (strstr(arg, "-u") == arg)
            unique = true;
        else if (strstr(arg, "-c") == arg)
            count = true;
        else if (strstr(arg, "-d") == arg)
            compare.delimiter = arg + 2;
        else if (strstr(arg, "-f") == arg) {
            size_t s;
            const char* p = advance(arg + 2, 0, ",", 1, &s);
            while (p && s) {
                compare.fields.push_back(make_pair(atoi(p)-1, p[s-1]));
                p = advance(p, 1, ",", 1, &s);
            }
        }
        else {
            cerr << "unknown argument " << arg << endl;
            return 1;
        }
    }
    stream_t stream(cin);
    if (unique && count) {
        typedef map<const char*, size_t, compare_t> rows_t;
        rows_t rows(compare);
        while (const char* p = stream.next())
            rows[p]++;
        for (rows_t::const_iterator i=rows.begin(), e=rows.end(); i!=e; ++i)
            cout << i->second << '\t' << i->first << endl;
    }
    if (unique) {
        typedef set<const char*, compare_t> rows_t;
        rows_t rows(compare);
        while (const char* p = stream.next())
            rows.insert(p);
        for (rows_t::const_iterator i=rows.begin(), e=rows.end(); i!=e; ++i)
            puts(*i);
    }
    else {
        typedef multiset<const char*, compare_t> rows_t;
        rows_t rows(compare);
        while (const char* p = stream.next())
            rows.insert(p);
        for (rows_t::const_iterator i=rows.begin(), e=rows.end(); i!=e; ++i)
            puts(*i);
    }
    return 0;
}
