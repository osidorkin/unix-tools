#include "stream.h"
#include <memory.h>

using namespace std;

stream_t::stream_t(istream& i) : in(i), chunkBuffer(0), chunkRemain(0) {}

const char* stream_t::next() {
    char* p = 0;
    if (getline(in, s)) {
        if (chunkRemain <= s.size() && s.size() < chunkSize) {
            chunkBuffer = new char[chunkSize];
            chunkRemain = chunkSize;
        }
        if (chunkRemain > s.size()) {
            p = chunkBuffer;
            chunkRemain -= s.size() + 1;
            chunkBuffer += s.size() + 1;
        }
        else
            p = new char[s.size() + 1];
        memcpy(p, s.data(), s.size());
        p[s.size()] = 0;
    }
    return p;
}
