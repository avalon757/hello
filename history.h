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

class fieldsbase
{
public:
    fieldsbase() {}
    fields_t &getfields() {
        return fields;
    }
    vector<string> fieldsname;

protected:
    fields_t fields;
};

template <class T>
void insfieldhistory(fieldsbase &f, string fname, T var, date dt) {
    f.getfields()[fname].push_back(make_pair(var, dt));
    return;
}

//const char *shipinfo_fieldname[] = {
//const vector<string> ship_fn = {
//        "name",
//        "zd",
//        "zzd",
//        "gl"
//};

class shipinfo : public fieldsbase
{
public:
    vector<string> fieldsname = {
            "name", "zd", "zzd", "gl"
    };
    shipinfo() {
        // init fields' name
        field_history_t v;
        for (auto fn : fieldsname)
            fields[fn] = v;     // fields[string("name")] = v;
    };
};


#endif //HELLO_HISTORY_BASE_H
