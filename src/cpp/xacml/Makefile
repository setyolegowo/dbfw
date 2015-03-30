FILES:=test.o

CPP=g++
CFLAGS:=-Wall -DPARSER_DEBUG -g
CXXFLAGS:=-g -Wall

xacml: $(FILES)
	$(CXX) $(CXXFLAGS) -o xacml $(FILES)

clean:
	rm -rf *.o
