//
// Created by wang on 18-2-27.
//

#ifndef HELLO_HISTORY_BASE_H
#define HELLO_HISTORY_BASE_H

#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

#include <boost/variant.hpp>
using boost::variant;

#include <unordered_map>
#include <utility>
using namespace std;
//using std::pair;
//using std::vector;
//using std::map;
//using std::string;
//using std::unordered_map;

/*
typedef vector<pair<string, date>> vtstr;
typedef vector<pair<int, date>> vtint;
typedef vector<pair<double, date>> vtdbl;
typedef vector<pair<date, date>> vtdt;

typedef boost::variant<vtstr, vtint, vtdbl, vtdt> vttype;
*/

typedef boost::variant<int, double, string, date> vartype;
typedef pair<vartype, date> vptype;
typedef vector<vptype> vttype;
typedef unordered_map<string, vttype> arftype;

class shipinfo
{
public:
    shipinfo() {
        vttype v = {make_pair(string("长福338"), date(1970,1,1))};
        vt[string("name")] = v;
    };
    const arftype &getvt() {
        return vt;
    }

//private:
    arftype vt;
};


#endif //HELLO_HISTORY_BASE_H
