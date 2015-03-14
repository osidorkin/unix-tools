TOOLS=uniq2 setops sortby

CC=g++

all: $(TOOLS)

stream.cc: stream.h

$(TOOLS): stream.cc

%: %.cc
	$(CC) -O3 $< stream.cc -o $@
	chmod +x $@

clean:
	rm -f $(TOOLS)

install:
	mkdir -p ~/bin
	cp $(TOOLS) ~/bin
