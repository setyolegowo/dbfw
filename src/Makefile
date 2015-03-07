OBJS:=mysql/mysql_con.o connection.o proxymap.o config.o dbfw.o log.o misc.o buffer.o main.o

# CPP=g++
CXXFLAGS:=-g -Wall
LIB=-lev

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
