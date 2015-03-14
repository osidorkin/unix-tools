#include "stream.h"
#include <memory.h>

using namespace std;

const size_t stream_t::chunkSize = 10*1024*1024;

stream_t::stream_t(istream& i) : in(i), chunkBuffer(0), chunkRemain(0) {}

const char* stream_t::next() {
    char* p = 0;
    if (getline(in, tmp)) {
        size_t size = tmp.size();
        if (chunkRemain <= size && size < chunkSize) {
            chunkBuffer = new char[chunkSize];
            chunkRemain = chunkSize;
        }
        if (chunkRemain > size) {
            p = chunkBuffer;
            chunkRemain -= size + 1;
            chunkBuffer += size + 1;
        }
        else
            p = new char[size + 1];
        memcpy(p, tmp.data(), size);
        p[size] = 0;
    }
    return p;
}
