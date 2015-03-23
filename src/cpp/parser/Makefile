FILES:= sql.o sql.tab.o parser.o

CPP=g++
CFLAGS:=-Wall -DPARSER_DEBUG -g
#ifndef CXXFLAGS
CXXFLAGS:=-g -DPARSER_DEBUG -Wall
#endif

#ifdef CXXFLAGS
objs: $(FILES)
#else
parser: $(FILES)
	$(CXX) $(CXXFLAGS) -o parser $(FILES)
#endif

sql.cpp:
	flex -i -osql.cpp sql.lex
	bison -d -v sql.ypp
	
sql.tab.cpp:

clean:
	rm -rf parser sql.tab.hpp sql.output sql.tab.cpp sql.cpp *.o
