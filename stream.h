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
};
