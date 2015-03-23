OBJS:=log.o misc.o buffer.o normalization.o config.o proxymap.o patterns.o dbfw.o connection.o mysql/mysql_con.o main.o
OOBJS:=parser/parser.o parser/sql.o parser/sql.tab.o

# CPP=g++
CXXFLAGS:=-g -Wall
LIB=-lev -lpcrecpp -lpcre

dbfw: $(OBJS)
	cd parser; make CXXFLAGS="$(CXXFLAGS)"
	$(CXX) $(CXXFLAGS) -o dbfw $(OBJS) $(OOBJS) $(LIB)

clean:
	rm -rf dbfw *.o mysql/*.o
	cd parser; make clean
