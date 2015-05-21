OBJS:=log.o misc.o buffer.o normalization.o config.o proxymap.o patterns.o dbfw.o connection.o dbperm.o mysql/mysql_con.o pgsql/pgsql_con.o main.o
OOBJS:=parser/parser.o parser/sql.o parser/sql.tab.o

# CPP=g++
CXXFLAGS:=-std=c++11 -g -Wall
LIB=-lev -lpcrecpp -lpcre

dbfw: $(OBJS)
	cd parser; make CXXFLAGS="$(CXXFLAGS)"
	$(CXX) $(CXXFLAGS) -o dbfw $(OBJS) $(OOBJS) $(LIB)

clean:
	rm -rf dbfw *.o mysql/*.o pgsql/*.o
	cd parser; make clean
