tools=uniq2 setops sortby

all: $(tools)

%: %.cc
	g++ -O3 $< -o $@
	chmod +x $@

clean:
	rm -f $(tools)

install:
	cp $(tools) ~/bin
