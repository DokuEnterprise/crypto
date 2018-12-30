# g++ main.cpp `pkg-config --cflags glib-2.0` -lzepher
CC = g++

CPPFLAGS= -std=c++14 -fomit-frame-pointer -fPIC -DQHASM 
LDFLAGS= -shared -L/usr/local/lib  -lpbc -lgmp

SOURCES = $(shell echo *.cpp)
HEADERS = $(shell echo *.hpp)
OBJECTS=$(SOURCES:.cpp=.o)

TARGET=libibe.so

all: $(TARGET)

.PHONY : clean
clean:
	rm -f $(OBJECTS)  $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

install:
	cp $(HEADERS) /usr/local/include/ibe
	cp libibe.so /usr/local/lib