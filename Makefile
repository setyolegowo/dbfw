CPP=g++
CFLAGS=-std=c++11

dbfw:
	rm -f src/cpp/dbfw
	cd src/cpp; make CPP="$(CPP)"; cp dbfw ../../

optimize:
	rm -f dbfw
	rm -f src/cpp/dbfw
	cd src/cpp; make CPP="$(CPP)" CXXFLAGS:="-std=c++11 -O2"; cp dbfw ../../

clean:
	cd src/cpp; make clean
	rm -rf dbfw
#	cd src; make clean; cd mysql; rm -rf *.o

