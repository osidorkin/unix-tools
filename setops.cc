#include <string>
#include <set>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <ext/stdio_filebuf.h>

using namespace std;

template <class T, class F>
void run(const set<T>& ss1, const set<T>& ss2, F algorithm) {
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

void usage() {
    cerr << "Usage: setops [operation] <[file1] 3<[file2]" << endl;
    cerr << "Available operations:" << endl;
    for (int i=0; i<4; ++i)
        cerr << "  " << operations[i] << endl;
    cerr << "You can use short prefix to specify operation like 'u' for 'union'." << endl;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        usage();
        return 1;
    }

    string s;
    set<string> ss1;
    while (getline(cin,s))
        ss1.insert(s);

    __gnu_cxx::stdio_filebuf<char> filebuf(3, std::ios::in);
    istream in2(&filebuf);

    set<string> ss2;
    while (getline(in2,s))
        ss2.insert(s);

    typedef set<string>::const_iterator I;
    typedef ostream_iterator<string> O;

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
