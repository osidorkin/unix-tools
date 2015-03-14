#include <string>
#include <istream>

struct stream_t {
	std::istream& in;
	static const size_t chunkSize = 10*1024*1024;
	char* chunkBuffer;
	size_t chunkRemain;
	std::string s;
	stream_t(std::istream&);

	const char* next();
};
