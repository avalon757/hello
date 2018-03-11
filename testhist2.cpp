//
// Created by wang on 18-3-3.
//

#include "history.h"
#include "history2.h"
using namespace std;
using boost::gregorian::years;
using boost::gregorian::days;

#include <boost/tokenizer.hpp>
#include <fstream>

using boost::escaped_list_separator;
using boost::tokenizer;

template <class T>
void prnitem(T elem) {
    cout << elem << " | ";
}

inline date_period make_dp(date first_point, date end_point) {
    return date_period(first_point, end_point);
}

inline date_period make_dp(const string &first_point, const string &end_point) {
    return date_period(from_string(first_point), from_string(end_point));
}

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

    typedef histnode<shiprec> shipnode_t;
    typedef shared_ptr<hisship_t> pship_t;

    pship_t p;
    if (p == nullptr)
        cout << "pship_t is null\n";
    p = make_shared<hisship_t>();
    shipnode_t sn;
    shiprec s("foo", 888.0, 1888.0, 0, 338);
    p->getvtdata().push_back(shipnode_t());
    p->getvtdata().push_back(shipnode_t(s));
    date_period dp = date_period(date(1977, 1, 1), days(100));
    p->getvtdata().push_back(shipnode_t(s, dp));
//    sn.dp = dpnull;
    sn.data = shiprec("foo", 777.0, 1747.0, 0, 234);
    sn.dp = date_period(date(1970, 1, 1), date(2000, 1, 1));
    p->getvtdata().push_back(sn);

    sn.data = shiprec("foo", 999.0, 1989.0, 60, 118);
    sn.dp = date_period(date(2000, 1, 1), date(2006, 1, 1));
    p->insnode(sn);
    p->insnode(shipnode_t(shiprec("One", 666.0, 1626.0, 0, 166), date_period(date(2006, 1, 1), date(2008, 1, 1))));
    try {
        p->insnode(shipnode_t(shiprec("Two", "3455.0", "2345.0", "0", "345"), date_period(date(2006, 1, 1), date(2008, 1, 1))));
    } catch (bad_lexical_cast &e) {
        cout << e.what() << endl;
    }

    p->print();

    shiprec rec(sn.data);
    cout << endl; rec.print(); cout << endl;

    shiprec const *ps = p->at(date(1999, 5, 4));
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

void test_history2a() {

    // typedef histnode<shiprec> snode_t;        // 同下
    typedef hisship_t::node_type snode_t;       // node type of ship history

    shared_ptr<hisship_t> paship = make_shared<hisship_t>();
    shiprec s("foo", 888.0, 1888.0, 88, 338);
    date_period dp(date(2000, 1, 1), date(2005, 7, 1));
    paship->insnode(snode_t(s, dp));
    s.name = "fooA"; s.zzd = 1988;
    paship->insnode(snode_t(s, date_period(date(2005, 7, 1), date(2008, 1, 1))));
    paship->insnode(snode_t(s, date_period(date(2008, 1, 1), date(2012, 1, 1))));
    paship->print();

    auto pa = paship->dateserial();
    void (*pfi)(date) = prnitem;
    for_each(pa->begin(), pa->end(), pfi);
    cout << endl;
    for_each(pa->begin(), pa->end(), [](date vi){ prnitem(vi); });
    cout << endl;
    for(auto v : *(pa)) {
        auto p = paship->at(v);
        if (nullptr != p)
            prnitem(*p);
    }
    cout << endl;

    shared_ptr<hisship_t> pbship = make_shared<hisship_t>();
    s = shiprec("fooB", 333.0, 338.0, 0, 123);
    pbship->insnode(snode_t(s, date_period(date(2006, 4, 1), date(2008, 12, 1))));
    pbship->insnode(snode_t(s, date_period(date(2008, 12, 1), date(2010, 12, 1))));
    pbship->insnode(snode_t(s, date_period(date(2010, 12, 1), date(2016, 1, 1))));
    pbship->print();

    auto pb = pbship->dateserial();     // 必须定义 shared_ptr 指针才可用，才不会被析构
    for(auto v : *(pb)) {
        prnitem(v);
        auto p = pbship->at(v);
        if (nullptr != p)
            prnitem(*p);
    }
    cout << endl;

    typedef hislicship_t::node_type lnode_t;
    shared_ptr<hislicship_t> palicship = make_shared<hislicship_t>();
    dp = date_period(date(2015, 10, 24), date(2017, 10, 24));
    licshiprec l(dp, "XX20150016", "Fu188", "fooCOMP", "Your Lisence Content is:...", 0x38);
    palicship->insnode(lnode_t(l, dp));
    dp = date_period(date(2017, 10, 12), date(2019, 10, 12));
    l.validperiod = dp;
    l.licno = "SS20170138";
    l.ynsj = false;
    l.ynsn = true;
    palicship->insnode(lnode_t(l, dp));
    l = licshiprec("2018-10-06", "2019-10-01", "BB20170338", "Fu38", "fooYYY", "Your Lisence...", "32");
    palicship->insnode(lnode_t(l, l.validperiod));
    palicship->print();
    cout << l.getyntype() << " - 0x" << hex << l.getyntype() << dec << endl;

    hislicship_t::base_type const *plic = palicship->at(date(2015, 9, 30));
    if (plic) cout << "plic: " << *plic << endl;
    else cout << "plic is nullptr.\n";
    plic = palicship->at(date(2017, 1, 1));
    if (plic) cout << "plic: " << *plic << endl;
    else cout << "plic is nullptr.\n";
    plic = palicship->at(date(2019, 1, 1));
    if (plic) cout << "plic: " << *plic << endl;
    else cout << "plic is nullptr.\n";
    plic = palicship->at(date(2019, 10, 1));
    if (plic) cout << "plic: " << *plic << endl;
    else cout << "plic is nullptr.\n";
}

void test_history2b() {

    typedef hisship_t::node_type snode_t;
    shared_ptr<hisship_t> paship = make_shared<hisship_t>();

    char buf[1024];
    ifstream ifile("ex.csv");
    escaped_list_separator<char> sep('\\', '\t', '\"');
    tokenizer<escaped_list_separator<char>> tok(string(buf), sep);
    vector<string> rec;
    while (ifile.getline(buf, 1024)) {
        string s(buf);
        tok.assign(s, sep);
        rec.clear();
        for (auto v : tok)
            rec.push_back(v);
        if (!rec.empty())
            paship->insnode(snode_t(shiprec(rec), make_dp(rec[5], rec[6])));
    }
    paship->print();
}