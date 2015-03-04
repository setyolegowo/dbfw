OBJS:=main.o config.o dbfw.o log.o misc.o

CPP=g++
CFLAGS:=-g -Wall

dbfw: $(OBJS)
# cd parser; make
# cd lib; make
	$(CXX) $(CFLAGS) -o dbfw $(OBJS)

# mysql/mysql_con.o:
# $(CXX) $(CFLAGS) -c mysql/mysql_con.cpp -o mysql/mysql_con.o

clean:
	rm -rf dbfw *.o
# rm -rf dbfw *.o mysql/*.o
# cd parser; make clean
# cd lib; make clean
