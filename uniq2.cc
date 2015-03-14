#include <string>
#include <map>
#include <iostream>
#include <string.h>
#include "stream.h"

using namespace std;

struct compare_t {
    bool operator()(const char* p, const char* q) const {
        return strcmp(p,q) < 0;
    }
};

int main(int argc, char** argv) {
    if (argc > 1) {
        cout << 
            "uniq2 filters out repeated lines in an input stream.\n"
            "Unlike unix uniq it can detect non-adjucent repeated lines i.e. there is no need to sort the lines first.\n"
            "As a bonus uniq2 prints lines in lexicographic order (byte representation is used for unicode characters).\n"
            "If you provide optional '-c' argument uniq2 will output number of occurances for each repeated line.\n";
        return 1;
    }

    typedef map<const char*, size_t, compare_t> rows_t;
    rows_t rows;
    stream_t stream(cin);
    while (const char* p = stream.next())
        ++rows[p];

    bool count = argc > 1 && strcmp(argv[1], "-c") == 0;
    for (rows_t::const_iterator i = rows.begin(), e=rows.end(); i!=e; ++i) {
        cout << i->first;
        if (count) {
            cout << "\t";
            cout << i->second;
        }
        cout << endl;
    }
}
