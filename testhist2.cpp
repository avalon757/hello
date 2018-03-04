//
// Created by wang on 18-3-3.
//

#include "history2.h"
using namespace std;
using boost::gregorian::years;
using boost::gregorian::days;
void test_history2()
{
    histship_t ship;
    typedef shared_ptr<histship_t> pship_t;
    pship_t p;
    if (p == nullptr)
        cout << "pship_t is null\n";
    p = make_shared<histship_t>();
    typedef histnode<shipbase> shipnode_t;
    shipnode_t sn;
    shipbase s("foo", 888.0, 1888.0, 338);
    p->getdtdata().push_back(shipnode_t());
    p->getdtdata().push_back(shipnode_t(s));
    date_period dp = date_period(date(1977, 1, 1), days(100));
    p->getdtdata().push_back(shipnode_t(s, dp));
//    sn.dp = dpnull;
    sn.data = shipbase("foo", 777.0, 1747.0, 234);
    sn.dp = date_period(date(1970, 1, 1), date(1999, 12, 31));
    p->getdtdata().push_back(sn);

    p->print();
}
