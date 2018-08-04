#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = g++
CFLAGS  = -g -Wall -std=c++11 -ldclxvi -lgmp -lssl -lcrypto -lboost_system

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: all

# To create the executable file count we need the object files
# countwords.o, counter.o, and scanner.o:
#
all:  test.o ibe.o conv.o
	$(CC) $(CFLAGS) -o power ibe.o conv.o test.o

test.o:  test.cpp
	$(CC) $(CFLAGS) -c test.cpp

ibe.o:  ibe.cpp 
	$(CC) $(CFLAGS) -c ibe.cpp

conv.o:  conv.cpp 
	$(CC) $(CFLAGS) -c conv.cpp





# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) count *.o *~