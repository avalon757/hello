//
// Created by wang on 18-3-3.
//

#include "history.h"
#include "history2.h"
#include <fstream>
using namespace std;
using boost::gregorian::years;
using boost::gregorian::days;

#include <boost/tokenizer.hpp>
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

void totalat(const vector<shared_ptr<hisship_t>> &vss, const date &dt) {

    vector<shared_ptr<shiprec>> vs;
    for (auto v : vss) {
        shared_ptr<shiprec> sps = v->at(dt);
        if (sps != nullptr)
            vs.push_back(sps);
    }
    shiprec ttl = shiprec();
    int n = 0;
    for (auto v : vs) {
        ttl.zd += v->zd;
        ttl.zzd += v->zzd;
        ttl.teu += v->teu;
        ttl.gl += v->gl;
        ++n;
    }
    cout << dt << " total number: " << n << endl;
    ttl.name = string("total ") + to_iso_extended_string(dt);
    ttl.print();
    cout << endl;
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
    shared_ptr<shiprec> s = make_shared<shiprec>("foo", 888.0, 1888.0, 0, 338, 1);
    p->getvtdata().push_back(shipnode_t());
    p->getvtdata().push_back(shipnode_t(s));
    date_period dp = date_period(date(1977, 1, 1), days(100));
    p->getvtdata().push_back(shipnode_t(s, dp));
//    sn.dp = dpnull;
    sn.data = make_shared<shiprec>("foo", 777.0, 1747.0, 0, 234, 2);
    sn.dp = date_period(date(1970, 1, 1), date(2000, 1, 1));
    p->getvtdata().push_back(sn);

    sn.data = make_shared<shiprec>("foo", 999.0, 1989.0, 60, 118, 0x8);
    sn.dp = date_period(date(2000, 1, 1), date(2006, 1, 1));
    p->insnode(sn);
    p->insnode(shipnode_t(make_shared<shiprec>("One", 666.0, 1626.0, 0, 166, 0x38), date_period(date(2006, 1, 1), date(2008, 1, 1))));
    try {
        p->insnode(shipnode_t(make_shared<shiprec>("Two", "3455.0", "2345.0", "0", "345", "111000"), date_period(date(2006, 1, 1), date(2008, 1, 1))));
    } catch (bad_lexical_cast &e) {
        cout << e.what() << endl;
    }

    p->print();

    shiprec rec(*(sn.data));
    cout << endl; rec.print(); cout << endl;

    shared_ptr<shiprec> ps = p->at(date(1999, 5, 4));
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
    shiprec s("foo", 888.0, 1888.0, 88, 338, 0x38);
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
    s = shiprec("fooB", 333.0, 338.0, 0, 123, 0x38);
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
    l.yn.ynsj = false;
    l.yn.ynsn = true;
    palicship->insnode(lnode_t(l, dp));
    l = licshiprec("2018-10-06", "2019-10-01", "BB20170338", "Fu38", "fooYYY", "Your Lisence...", "32");
    palicship->insnode(lnode_t(l, l.validperiod));
    palicship->print();
    cout << l.getyntype() << " - 0x" << hex << l.getyntype() << dec << endl;

    shared_ptr<hislicship_t::base_type> plic = palicship->at(date(2015, 9, 30));
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

    vector<shared_ptr<hisship_t>> vsship;
    typedef hisship_t::node_type snode_t;
    shared_ptr<hisship_t> pship;

    ifstream ifile("ex.csv");
    string line;
    escaped_list_separator<char> sep('\\', '\t', '\"');
    tokenizer<escaped_list_separator<char>> tok(line, sep);
    vector<string> rec;
    string sname("noname");
    while (getline(ifile, line)) {
        tok.assign(line, sep);
        rec.clear();
        for (auto v : tok)
            rec.push_back(v);
        if (!rec.empty()) {
            if (sname != rec[0]) {
                sname = rec[0];
                pship = make_shared<hisship_t>();
                vsship.push_back(pship);
            }
            pship->insnode(snode_t(make_shared<shiprec>(rec), make_dp(rec[6], rec[7])));
        }
        line.clear();
    }
//    for (auto v : vsship) {
//        v->print();
//        cout << "----- >< -----\n";
//    }

    totalat(vsship, date(2016, 1, 1));
    totalat(vsship, date(2017, 1, 1));
    totalat(vsship, date(2018, 1, 1));
}

void test_history3() {

    typedef hiscomp_t::node_type cnode_t;       // node type of company history

    vector<shared_ptr<hiscomp_t>> vscomp;
    shared_ptr<hiscomp_t> pcomp;

    ifstream ifile("excompany.csv");
    string line;
    escaped_list_separator<char> sep('\\', '\t', '\"');
    tokenizer<escaped_list_separator<char>> tok(line, sep);
    vector<string> rec;
    string sname("noname");
    while (getline(ifile, line)) {
        tok.assign(line, sep);
        rec.clear();
        for (auto v : tok)
            rec.push_back(v);
        if (!rec.empty()) {
            if (sname != rec[0]) {
                sname = rec[0];
                pcomp = make_shared<hiscomp_t>();
                vscomp.push_back(pcomp);
            }
            pcomp->insnode(cnode_t(make_shared<companyrec>(rec), make_dp(rec[6], rec[7])));
        }
        line.clear();
    }
    for (auto v : vscomp) {
        v->print();
        cout << "----- >< -----\n";
    }

}