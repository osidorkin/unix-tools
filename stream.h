#include <string>
#include <istream>

struct stream_t {
	std::istream& in;
	static const size_t chunkSize;
	char* chunkBuffer;
	size_t chunkRemain;
	std::string tmp;

	stream_t(std::istream&);
	const char* next();

	// stream_t memory model is chunked append-only stack in heap 
	// stream_t output lifetime is expected to be equal to program lifetime so no memory deallocation provided
};
