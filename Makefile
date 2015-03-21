CPP=g++

dbfw:
	rm -rf dbfw src/dbfw
	cd src; make CPP="$(CPP)"; cp dbfw ../

clean:
	cd src; make clean
	rm -rf dbfw
#	cd src; make clean; cd mysql; rm -rf *.o

