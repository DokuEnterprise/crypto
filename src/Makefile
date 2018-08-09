CC = g++

CPPFLAGS= -O3 -fomit-frame-pointer -fPIC -DQHASM
LDFLAGS= -shared -L/usr/local/lib -ldems -ldclxvi -lcrypto -lssl

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TEST_FLAGS = -L$(ROOT_DIR) -I$(ROOT_DIR) -ldems -ldclxvi -lcrypto -lssl

SOURCES = $(shell echo *.cpp)
HEADERS = $(shell echo *.h)
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=libdems.so

all: $(TARGET) testfiles

.PHONY : clean
clean:
	rm -f $(OBJECTS) test.o $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

testfiles:
	$(CC) test/test.cpp $(TEST_FLAGS) -o test.o