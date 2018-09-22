#include <dems/ibe.hpp>
#include <iostream>

using namespace std;

//g++ test.cpp -ldems -ldclxvi -lcrypto -lssl -lsodium -lboost_system
int main(){
    Ibe d;
    d.setup();
    d.extract("friedrichdoku");
    cout << "running" << endl;

    cout << d.private_key.s << endl;
    return 0;
}