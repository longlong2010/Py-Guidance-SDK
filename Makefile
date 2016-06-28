CFLAGS = -g -Wall -I /usr/include/python2.7/
LDFLAGS = -Wl,-rpath,./ -L. -lDJI_guidance -lusb-1.0 -lpthread -lrt -fpic --shared
CC = g++

all: guidance.so

guidance.so: py_guidance.cc DJI_utility.o
	$(CC) $(CFLAGS) $(LDFLAGS) py_guidance.cc DJI_utility.o -o guidance.so

DJI_utility.o: DJI_utility.cpp DJI_utility.h
	$(CC) $(CFLAGS) -fpic -c DJI_utility.cpp

clean:
	rm -f *.o
	rm -f guidance.so

test:
	python2 test.py
