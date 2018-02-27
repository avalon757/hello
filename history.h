//
// Created by wang on 18-2-27.
//

#ifndef HELLO_HISTORY_BASE_H
#define HELLO_HISTORY_BASE_H

#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;

#include <boost/variant.hpp>
using boost::variant;

using std::pair;
using std::vector;
using std::map;
using std::string;


typedef vector<pair<string, date>> vtstr;
typedef vector<pair<int, date>> vtint;
typedef vector<pair<double, date>> vtdbl;
typedef vector<pair<date, date>> vtdt;

typedef boost::variant<vtstr, vtint, vtdbl, vtdt> vttype;

class shipinfo
{
private:
    map<string, vttype> vt;
};


#endif //HELLO_HISTORY_BASE_H
