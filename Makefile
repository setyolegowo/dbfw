CPP=g++
CFLAGS=-std=c++11
DIST=$(CURDIR)/dist

dbfw: $(DIST)/dbfw

$(DIST)/dbfw:
	rm -f src/cpp/dbfw
	mkdir -p $(DIST)
	cd src/cpp; make CPP="$(CPP)" DIST="$(DIST)"

optimize:
	rm -f src/cpp/dbfw
	cd src/cpp; make CPP="$(CPP)" CXXFLAGS:="-std=c++11 -O2" DIST="$(DIST)"

clean:
	cd src/cpp; make clean
	rm -rf $(DIST)/dbfw
#	cd src; make clean; cd mysql; rm -rf *.o

