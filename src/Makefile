OBJS:=main.o

CPP=g++
CFLAGS:=-g -Wall

dbfw: $(OBJS)
	cd parser; make
	cd lib; make
	$(CXX) $(CFLAGS) -o dbfw $(OBJS) parser/parser.o parser/sql.o parser/sql.tab.o

mysql/mysql_con.o:
	$(CXX) $(CFLAGS) -c mysql/mysql_con.cpp -o mysql/mysql_con.o

clean:
	rm -rf dbfw *.o mysql/*.o
	cd parser; make clean
	cd lib; make clean
