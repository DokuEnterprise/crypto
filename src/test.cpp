#include "ibe.hpp"
#include <iostream>

using namespace std;

int main(){
    MasterKey d;
    d.setup();
    d.extract("friedrichdoku");

    cout << d.private_key.s << endl;
    return 0;
}