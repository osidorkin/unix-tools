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
        "sortby is fast implementation of in-memory text file sorting with ability to specify columns as a key for sort operation\n"
        "sortby reads data from standard input and writes data to standard output.\n"
        "Arguments:\n"
        "  -d delim - Delimiter to use to separate fields in source stream.\n"
        "  -f list - Defines combination of fields to be used as a a sorting key.\n"
        "             Your can use 'i' suffix to interpret field value as in integer.\n"
        "  -u - Leave only first occurance for lines with same key.\n"
        "  -c - In combination with -u print number of lines with the same key.\n"
        "  -s - Swap number of lines and line text in -c mode.\n"
        "\nExample: cat example.txt | sortby -d\\| -f3,1i\n"
        "   Will sort lines from pipe ('|') delimited file example.txt using combination of third\n"
        "   and fisrt columns as a sorting key. first column value will be interpreted as an integer.\n"
        "\nNote: if fields list is empty sortby will work as a faster version of unix sort utility.\n";
}

inline const char* advance(const char* p, int n, const char* d, size_t l, size_t* s) {
    while (p && n-- > 0) {
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

struct settings_t {
    compare_t compare;
    bool unique;
    bool count;
    bool swap;
    settings_t() : unique(false), count(false), swap(false) {}
    
    bool setup(int argc, char** argv);
};

int main(int argc, char** argv) {
    settings_t settings;
    if (!settings.setup(argc, argv))
        return 1;

    stream_t stream(cin);
    if (settings.unique && settings.count) {
        typedef map<const char*, size_t, compare_t> rows_t;
        rows_t rows(settings.compare);
        while (const char* p = stream.next()) {
            pair<rows_t::iterator, bool> r = rows.insert(make_pair(p,1));
            if (!r.second) {
                ++r.first->second;
                stream.undo(p);
            }
        }
        for (rows_t::const_iterator i=rows.begin(), e=rows.end(); i!=e; ++i) {
            if (settings.swap)
                cout << i->first << '\t' << i->second << endl;
            else
                cout << i->second << '\t' << i->first << endl;
        }
    }
    else if (settings.unique) {
        typedef set<const char*, compare_t> rows_t;
        rows_t rows(settings.compare);
        while (const char* p = stream.next())
            if (!rows.insert(p).second)
                stream.undo(p);
        for (rows_t::const_iterator i=rows.begin(), e=rows.end(); i!=e; ++i)
            puts(*i);
    }
    else {
        typedef multiset<const char*, compare_t> rows_t;
        rows_t rows(settings.compare);
        while (const char* p = stream.next())
            rows.insert(p);
        for (rows_t::const_iterator i=rows.begin(), e=rows.end(); i!=e; ++i)
            puts(*i);
    }
    return 0;
}

bool settings_t::setup(int argc, char** argv) {
    for (int i=1; i<argc; ++i) {
        const char* arg = argv[i];
        if (strstr(arg, "-h") == arg || strstr(arg, "--h") == arg) {
            help();
            return false;
        }
        else if (strcmp(arg, "-u") == 0)
            unique = true;
        else if (strcmp(arg, "-c") == 0)
            count = true;
        else if (strcmp(arg, "-s") == 0)
            swap = true;
        else if (strstr(arg, "-d") == arg) {
            if (strlen(arg) > 2)
                arg += 2;
            else if (i + 1 < argc)
                arg = argv[++i];
            else {
                cerr << "delimiter is missing after -d argument" << endl;
                return 1;
            }
            compare.delimiter = arg;
        }
        else if (strstr(arg, "-f") == arg) {
            if (strlen(arg) > 2)
                arg += 2;
            else if (i + 1 < argc)
                arg = argv[++i];
            else {
                cerr << "field list is missing after -f argument" << endl;
                return 1;
            }
            size_t s;
            const char* p = advance(arg, 0, ",", 1, &s);
            while (p && s) {
                compare.fields.push_back(make_pair(atoi(p)-1, p[s-1]));
                p = advance(p, 1, ",", 1, &s);
            }
        }
        else {
            cerr << "unknown argument " << arg << endl;
            return false;
        }
    }
    return true;
}
