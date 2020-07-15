.PHONY : clean

SSL_L=/usr/local/openssl-1.0/lib64
INC1 = include
INC2 = /usr/local/openssl-1.0/include
#d =  -I$(INC2) -I$(INC1) -L$(SSL_L) -lssl -lcrypto -lgmp  -DNDEBUG -pipe -O3 -march=x86-64 -Wall -fomit-frame-pointer -ffast-math -funroll-loops -fPIC  -g # C flags

CC = gcc

CPPFLAGS= -I$(INC2) -I$(INC1)  -fomit-frame-pointer -fPIC -DQHASM
LDFLAGS= -shared  -I$(INC2) -I$(INC1) -L$(SSL_L) -lcrypto -lssl -lgmp
SOURCES = $(shell echo *.c)
HEADERS = $(shell echo include/*.h)
OBJECTS=$(SOURCES:.c=.o)

TARGET=libsibe.so

all: $(TARGET)

.PHONY : clean
clean:
	rm -f $(OBJECTS)  $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

install:
	cp $(HEADERS) /usr/local/include/sibe
	cp libsibe.so /usr/local/lib

