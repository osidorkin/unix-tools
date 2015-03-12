#include <string>
#include <map>
#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char** argv) {
    string s;
    map<string,long> ss;
    while (getline(cin,s))
        ++ss[s];

    bool f = argc > 1 && strcmp(argv[1], "-c") == 0;
    for (map<string,long>::const_iterator i = ss.begin(); i != ss.end(); ++i) {
        cout << i->first;
        if (f) {
            cout << "\t";
            cout << i->second;
        }
        cout << endl;
    }
}
