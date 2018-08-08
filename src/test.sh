# Works 

g++ -shared -Wl,-soname=libdems.so -L/usr/local/lib -lssl -lcrypto -ldclxvi -o libdems.so -O3 -fomit-frame-pointer -fPIC -DQHASM conv.cpp ibe.cpp utils.cpp
g++ test.cpp -L/home/professor/DEMS/crypt/src -ldems -ldclxvi -lcrypto -lssl
./a.out