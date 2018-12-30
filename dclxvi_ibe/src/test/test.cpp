#include <dems/ibe.hpp>
#include <iostream>

using namespace std;

//g++ test.cpp -ldems -ldclxvi -lcrypto -lssl -lsodium -lboost_system

void testSTARTUP(){
    Ibe d;
    d.setup();
    cout << "SETUP PASSED" << endl;

    d.extract("friedrichdoku");
    cout << d.id_private_key.d << endl;
    cout << d.id_private_key.q << endl;
    cout << "EXTRACT PASSED" << endl;

    cout << d.private_key.s << endl;
    cout << "PRIVATE KEY PASSED" << endl;
}

void testENCDEC(){
    Ibe d;
    d.setup();
    d.extract("fatrick@gmail.com");

    struct cipherdata x = d.encrypt("fatrick@gmail.com", "hello");
    d.decrypt(d.id_private_key,x);

    //struct cipherdata y = d.encrypt("ethan", "scholar");
}

int main(){
    testENCDEC();
    return 0;
}

