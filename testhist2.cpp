//
// Created by wang on 18-3-3.
//

#include "history.h"
#include "history2.h"
using namespace std;
using boost::gregorian::years;
using boost::gregorian::days;

void test_history1() {

    shipinfo s = shipinfo();
    s.insfieldhistory("name", "长福338", date(1970,1,1));
    s.insfieldhistory("name", "长兴388", date(1990,12,1));
    s.insfieldhistory("zd", 988.0, date(1970,1,2));
    s.insfieldhistory("zzd", 1088.0, date(1970,1,3));
    s.insfieldhistory("zzd", 1188.0, date(1990,12,1));
    s.insfieldhistory("gl", 750, date(1970,1,4));
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
}

void test_history2() {

    histship_t ship;
    typedef shared_ptr<histship_t> pship_t;
    pship_t p;
    if (p == nullptr)
        cout << "pship_t is null\n";
    p = make_shared<histship_t>();
    typedef histnode<shipbase> shipnode_t;
    shipnode_t sn;
    shipbase s("foo", 888.0, 1888.0, 338);
    p->getvtdata().push_back(shipnode_t());
    p->getvtdata().push_back(shipnode_t(s));
    date_period dp = date_period(date(1977, 1, 1), days(100));
    p->getvtdata().push_back(shipnode_t(s, dp));
//    sn.dp = dpnull;
    sn.data = shipbase("foo", 777.0, 1747.0, 234);
    sn.dp = date_period(date(1970, 1, 1), date(2000, 1, 1));
    p->getvtdata().push_back(sn);

    sn.data = shipbase("foo", 999.0, 1989.0, 118);
    sn.dp = date_period(date(2000, 1, 1), date(2006, 1, 1));
    p->insnode(sn);
    p->print();

    shipbase const *ps = p->at(date(1999, 5, 4));
    if (ps) ps->print();
    cout << endl;
    ps = p->at(date(2003, 5, 4));
    if (ps) ps->print();
    cout << endl;
    ps = p->at(date(2013, 5, 4));
    if (ps) ps->print();
    else cout << "date,2013-5-4 is null.";
    cout << endl;
}
