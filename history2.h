//
// Created by wang on 18-3-3.
//

#ifndef HELLO_HISTORY2_H
#define HELLO_HISTORY2_H

#include <boost/date_time/gregorian/gregorian.hpp>
//using namespace boost::gregorian;
//namespace bdt = boost::date_time;
//using namespace boost::date_time;
using boost::gregorian::date;
using boost::gregorian::date_period;
using boost::date_time::neg_infin;

using std::string;
using std::vector;
using std::shared_ptr;


const date_period dpnull = date_period(date(neg_infin), date(neg_infin));

// 时间段
class histbase
{
public:
    explicit histbase() : dp(dpnull) {}
    explicit histbase(date dtbegin, date dtend) : dp(dtbegin, dtend) {}
    explicit histbase(const date_period &dtprd) : dp(dtprd) {}
    bool contains(date dt) { return dp.contains(dt); }

    date_period dp;
};

// 时间段内的数据
template <class T>
class histnode : public histbase
{
public:
    histnode()
            : histbase(dpnull), data() {}
    histnode(const T &d, const date_period &vdp = dpnull)
            : data(d), histbase(vdp) {}
    const T &at(const date &dt) { return data; }
//    T &get() { return data; }

    T data;
};

// 历史记录，一串时间段内的数据
template <class T>
class history2
{
public:
    typedef vector<histnode<T> > vhist;
    vhist &getdtdata() { return vtdata; };
    void print() {
        for (auto v : vtdata) {
            std::cout << v.dp << " -> ";
            v.data.print();
            std::cout << std::endl;
        }
    };

protected:
    vhist vtdata;
};

// 数据定义
class licshipbase
{
public:
    date_period validperiod;
    string compname;
    double zd;
    double zzd;
    string content;
};
typedef history2<licshipbase> histlicship_t;

class shipbase
{
public:
    shipbase() : name(), zd(), zzd(), gl() {}
    shipbase(const string &vname, double vzd, double vzzd, int vgl)
            : name(vname), zd(vzd), zzd(vzzd), gl(vgl) {}

    void print() {
        std::cout << "(" << name << ", " << zd
                  << ", " << zzd << ", " << gl << ")";
    }

    string name;
    double zd;
    double zzd;
    int gl;
    shared_ptr<histlicship_t> lic;
    shared_ptr<histlicship_t> hklic;
};
typedef history2<shipbase> histship_t;



class compbase
{
    string name;
    string addr;
    string legalman;
};




//class histbase
//{
//    date_period dp;
//    bool contains(const date &dt) { return dp.contains(dt); }
//};
//
//class shipbase
//{
//    string name;
//    double zd;
//    double zzd;
//    int gl;
//};
//
//class histship : public histbase, shipbase
//{
//
//};

#endif //HELLO_HISTORY2_H
