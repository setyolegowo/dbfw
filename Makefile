CPP=g++

dbfw:
	rm -rf dbfw src/cpp/dbfw
	cd src/cpp; make CPP="$(CPP)"; cp dbfw ../../

clean:
	cd src/cpp; make clean
	rm -rf dbfw
#	cd src; make clean; cd mysql; rm -rf *.o

