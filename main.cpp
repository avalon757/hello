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
    fields_t &flds = s.getvt();
    field_history_t fldhist = flds["name"];
    field_tp_t fldtp = *(fldhist.begin());
    field_t fld = fldtp.first;
    date dt = fldtp.second;
    cout << get<string>(fld) << " # " << dt << endl;
    cout << get<string>(flds["name"].begin()->first)
         << " # " << flds["name"].begin()->second << endl;

    cout << t.elapsed() << endl;
    return 0;
}