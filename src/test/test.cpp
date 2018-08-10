#include "ibe.hpp"
#include <iostream>

using namespace std;

int main(){
    Ibe d;
    d.setup();
    d.extract("friedrichdoku");
    cout << "running" << endl;

    cout << d.private_key.s << endl;
    return 0;
}