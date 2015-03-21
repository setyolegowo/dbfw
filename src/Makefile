OBJS:=log.o misc.o buffer.o normalization.o config.o proxymap.o patterns.o dbfw.o connection.o mysql/mysql_con.o main.o

# CPP=g++
CXXFLAGS:=-g -Wall
LIB=-lev -lpcrecpp -lpcre

dbfw: $(OBJS)
# cd parser; make
# cd lib; make
	$(CXX) $(CXXFLAGS) -o dbfw $(OBJS) $(LIB)

# mysql/mysql_con.o:
# $(CXX) $(CFLAGS) -c mysql/mysql_con.cpp -o mysql/mysql_con.o

clean:
	rm -rf dbfw *.o mysql/*.o
# cd parser; make clean
# cd lib; make clean
