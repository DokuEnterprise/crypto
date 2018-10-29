#include <dems/ibe.hpp>
#include <iostream>

using namespace std;

//g++ test.cpp -ldems -ldclxvi -lcrypto -lssl -lsodium -lboost_system
int main(){
    Ibe d;
    d.setup();
    cout << "SETUP PASSED" << endl;

    d.extract("friedrichdoku");
    cout << d.id_private_key.d << endl;
    cout << d.id_private_key.q << endl;
    cout << "EXTRACT PASSED" << endl;

    cout << d.private_key.s << endl;
    cout << "PRIVATE KEY PASSED" << endl;

    struct cipherdata x = d.encrypt("friedrichdoku", "hello");
    cout << "CIPHER " << x.ciphertext << endl;
    
    d.decrypt(&d.id_private_key,x);


    return 0;
}