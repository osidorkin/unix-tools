tools=uniq2 setops

all: $(tools)

%: %.cc
	g++ -O3 $< -o $@
	chmod +x $@

clean:
	rm -f $(tools)

install:
	cp $(tools) ~/bin
