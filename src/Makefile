CC = g++

CPPFLAGS= -std=c++11 -fomit-frame-pointer -fPIC -DQHASM
LDFLAGS= -shared -L/usr/local/lib -ldclxvi -lcrypto -lssl -lsodium -lgmp -lboost_system

ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
TEST_FLAGS = -L$(ROOT_DIR) -I$(ROOT_DIR) -ldems -ldclxvi -lcrypto -lssl -lsodium -lboost_system

SOURCES = $(shell echo *.cpp)
HEADERS = $(shell echo *.hpp)
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=libdems.so

all: $(TARGET) testfiles

.PHONY : clean
clean:
	rm -f $(OBJECTS) test.o $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

testfiles:
	$(CC) test/test.cpp $(TEST_FLAGS) -o test/test.o

install:
	cp $(HEADERS) /usr/local/include/dems
	cp libdems.so /usr/local/lib

install_test:
	cp /usr/local/lib/libdclxvi.so $(ROOT_DIR)/test
	cp /usr/local/lib/libdems.so $(ROOT_DIR)/test
	cp /usr/local/lib/libsodium.so $(ROOT_DIR)/test