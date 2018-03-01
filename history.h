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

typedef boost::variant<vtstr, vtint, vtdbl, vtdt> field_history_t;
*/

typedef boost::variant<int, double, string, date> field_t;
typedef pair<field_t, date> field_tp_t;
typedef vector<field_tp_t> field_history_t;
typedef unordered_map<string, field_history_t> fields_t;

class shipinfo
{
public:
    shipinfo() {
        field_history_t v = {make_pair("长福338", date(1970,1,1))};
        vt[string("name")] = v;
    };
    fields_t &getvt() {
        return vt;
    }

private:
    fields_t vt;
};


#endif //HELLO_HISTORY_BASE_H
