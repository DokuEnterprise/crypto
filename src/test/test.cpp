#include <dems/ibe.hpp>
#include <iostream>

using namespace std;

//g++ test.cpp -ldems -ldclxvi -lcrypto -lssl -lsodium -lboost_system
int main(){
    Ibe d;
    d.setup();
    cout << "SETUP PASSED" << endl;
    d.extract("friedrichdoku");
    cout << "EXTRACT PASSED" << endl;

    cout << d.private_key.s << endl;
    cout << "TEST PASSED" << endl;
    return 0;
}