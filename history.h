//
// Created by wang on 18-2-27.
//

#ifndef HELLO_HISTORY_BASE_H
#define HELLO_HISTORY_BASE_H

#include <boost/date_time/gregorian/gregorian.hpp>
//using namespace boost::gregorian;
using boost::gregorian::date;

#include <boost/variant.hpp>
using boost::variant;

#include <unordered_map>
#include <utility>
using std::unordered_map;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;

/*
typedef vector<pair<string, date>> vtstr;
typedef vector<pair<int, date>> vtint;
typedef vector<pair<double, date>> vtdbl;
typedef vector<pair<date, date>> vtdt;

typedef boost::variant<vtstr, vtint, vtdbl, vtdt> field_history_t;
*/

typedef boost::variant<bool, int, double, string, date> field_t;
typedef pair<field_t, date> field_tp_t;
typedef vector<field_tp_t> field_history_t;
typedef unordered_map<string, field_history_t> fields_t;

class fieldsbase
{
public:
    template <class T>
    void insfieldhistory(const string &fname, T var, date dt) {
        fields[fname].push_back(make_pair(var, dt));
    }

    fields_t &getfields() {
        return fields;
    }
    const vector<string> fieldsname;

protected:
    fieldsbase() {}     // 只能继承使用
    fields_t fields;
};

/*
template <class T>
void insfieldhistory(fieldsbase &f, string fname, T var, date dt) {
    f.getfields()[fname].push_back(make_pair(var, dt));
    return;
}
*/

class shipinfo : public fieldsbase
{
public:
    const vector<string> fieldsname = {
            "id", "coid", "name", "cjg", "yyzbh", "yyzfzrq", "yyzyxq", "yyzarid",
            "goyyzbh", "goyyzfzrq", "goyyzyxq", "goyyzarid", "stid", "cbjcrq",
            "cbzd", "cbzzd", "cbteu", "cblfm", "cbcw", "cbkw", "cbgl", "cbjyfw",
            "yngo", "ynjnm", "ynsj", "ynsn", "ynssj", "ynssn", "syr", "glr", "yntg",
            "bz", "lsarid"
    };
    shipinfo() {
        // init fields' name
        field_history_t v;
        for (auto fn : fieldsname)
            fields[fn] = v;     // fields[string("name")] = v;
    };
};


#endif //HELLO_HISTORY_BASE_H
