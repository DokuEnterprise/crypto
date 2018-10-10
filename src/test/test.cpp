#include <dems/ibe.hpp>
#include <iostream>

using namespace std;

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