#include <string>
#include <set>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <ext/stdio_filebuf.h>
#include "stream.h"

using namespace std;

struct compare_t {
    bool operator()(const char* p, const char* q) const {
        return strcmp(p,q) < 0;
    }
};

template <class T, class C, class F>
void run(const set<T,C>& ss1, const set<T,C>& ss2, F algorithm) {
    algorithm(
        ss1.begin(), ss1.end(),
        ss2.begin(), ss2.end(),
        ostream_iterator<T>(cout, "\n"));
}

const char* operations[] = {
    "union",
    "difference",
    "intersection",
    "symmetric_difference"
};

void help() {
    cout << "setops provides set operations for text files" << endl;
    cout << "Usage: setops [operation] <[file1] 3<[file2]" << endl;
    cout << "Available operations:" << endl;
    for (int i=0; i<4; ++i)
        cout << "  " << operations[i] << endl;
    cout << "You can use short prefix to specify operation like 'u' for 'union'." << endl;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        help();
        return 1;
    }

    typedef set<const char*, compare_t> rows_t;

    rows_t ss1;
    stream_t in1(cin);
    while (const char* p = in1.next())
        ss1.insert(p);

    rows_t ss2;
    __gnu_cxx::stdio_filebuf<char> filebuf(3, std::ios::in);
    istream cin3(&filebuf);
    stream_t in2(cin3);
    while (const char* p = in2.next())
        ss2.insert(p);

    typedef rows_t::const_iterator I;
    typedef ostream_iterator<const char*> O;

    for (int i=0; i<4; ++i) {
        if (strstr(operations[i], argv[1]) == operations[i]) {
            switch (i) {
                case 0: run(ss1, ss2, set_union<I,I,O>); return 0;
                case 1: run(ss1, ss2, set_difference<I,I,O>); return 0;
                case 2: run(ss1, ss2, set_intersection<I,I,O>); return 0;
                case 3: run(ss1, ss2, set_symmetric_difference<I,I,O>); return 0;
            }
        }
    }
    cerr << "Unknown operation." << endl;
    return 2;
}
