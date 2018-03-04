#include <iostream>
#include <boost/timer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace std;
using namespace boost::gregorian;

#include "history.h"
#include "history2.h"
extern void test_history2();

int main() {
    boost::timer t;
    std::cout << "Hello, World!" << std::endl;

    shipinfo s = shipinfo();
    insfieldhistory(s, "name", "长福338", date(1970,1,1));
    insfieldhistory(s, "name", "长兴388", date(1990,12,1));
    insfieldhistory(s, "zd", 988.0, date(1970,1,2));
    insfieldhistory(s, "zzd", 1088.0, date(1970,1,3));
    insfieldhistory(s, "zzd", 1188.0, date(1990,12,1));
    insfieldhistory(s, "gl", 750, date(1970,1,4));
/*
    fields_t &flds = s.getfields();
    field_history_t fldhist = flds["name"];
    field_tp_t fldtp = *(fldhist.begin());
    field_t fld = fldtp.first;
    date dt = fldtp.second;
    cout << fld << " # " << dt << endl;
    cout << get<string>(fld) << " # " << dt << endl;
    cout << get<string>(flds["name"].begin()->first)
         << " # " << flds["name"].begin()->second << endl;
*/
    for (auto v : s.getfields()) {
        cout << v.first << ": ";
        for (auto f : v.second)
            cout << "(" << f.first << ", " << f.second << "), ";
        cout << endl;
    }

    test_history2();

    cout << t.elapsed() << endl;
    return 0;
}