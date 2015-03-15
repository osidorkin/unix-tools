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

void help() {
    cout <<
        "uniq2 filters out repeated lines in an input stream.\n"
        "Unlike unix uniq it can detect non-adjucent repeated lines i.e. there is no need to sort the lines first.\n"
        "As a bonus uniq2 prints lines in lexicographic order (byte representation is used for unicode characters).\n"
        "Arguments:\n"
        "  -c - (optional) output number of occurances for each repeated line.\n"
        "  -s - (optional) swap number of occurances and line text in -c mode.\n";
}

int main(int argc, char** argv) {
    bool count = false;
    bool swap = false;
    for (int i=1; i<argc; ++i) {
        const char* arg = argv[i];
        if (strstr(arg, "-h") == arg || strstr(arg, "--h") == arg) {
            help();
            return 1;
        }
        if (strcmp(arg, "-c") == 0)
            count = true;
        else if (strcmp(arg, "-s") == 0)
            swap = true;
        else {
            cerr << "unknown argument " << arg << endl;
            return 1;
        }
    }

    typedef map<const char*, size_t, compare_t> rows_t;
    rows_t rows;
    stream_t stream(cin);
    while (const char* p = stream.next()) {
        pair<rows_t::iterator, bool> r = rows.insert(make_pair(p,1));
        if (!r.second) {
            ++r.first->second;
            stream.undo(p);
        }
    }

    for (rows_t::const_iterator i = rows.begin(), e=rows.end(); i!=e; ++i) {
        if (count && !swap)
            cout << i->second << '\t';
        cout << i->first;
        if (count && swap)
            cout << '\t' << i->second;
        cout << endl;
    }

    return 0;
}
