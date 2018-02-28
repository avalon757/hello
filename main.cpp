#include <iostream>
#include <boost/timer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace std;
using namespace boost::gregorian;

#include "history.h"

int main() {
    boost::timer t;
    std::cout << "Hello, World!" << std::endl;

    shipinfo s = shipinfo();
    arftype &arf = s.vt;
    vttype vvp = arf["name"];
    vptype vp = *(vvp.begin());
    vartype v = vp.first;
    date dt = vp.second;
    cout << get<string>(v) << " # " << dt << endl;
    cout << get<string>(arf["name"].begin()->first)
         << " # " << arf["name"].begin()->second << endl;

    cout << t.elapsed() << endl;
    return 0;
}