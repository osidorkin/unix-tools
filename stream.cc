#include "stream.h"
#include <memory.h>

using namespace std;

const size_t stream_t::chunkSize = 10*1024*1024;

stream_t::stream_t(istream& i) : in(i), chunkBuffer(0), chunkRemain(0) {}

const char* stream_t::next() {
    char* p = 0;
    if (getline(in, tmp)) {
        if (chunkRemain <= tmp.size() && tmp.size() < chunkSize) {
            chunkBuffer = new char[chunkSize];
            chunkRemain = chunkSize;
        }
        if (chunkRemain > tmp.size()) {
            p = chunkBuffer;
            chunkRemain -= tmp.size() + 1;
            chunkBuffer += tmp.size() + 1;
        }
        else
            p = new char[tmp.size() + 1];
        memcpy(p, tmp.data(), tmp.size());
        p[tmp.size()] = 0;
    }
    return p;
}
