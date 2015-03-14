#include <string>
#include <istream>

struct stream_t {
	std::istream& in;
	static const size_t chunkSize;
	char* chunkBuffer;
	size_t chunkRemain;
	std::string tmp;
	const char* last;

	stream_t(std::istream&);
	const char* next();
	void undo(const char*); // 1-step undo operation

	// stream_t memory model is chunked append-only stack in heap 
	// stream_t output lifetime is expected to be equal to program lifetime so no memory deallocation provided
	// stream_t provides 1-step undo operation to push back last allocated element
};
