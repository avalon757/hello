//
// Created by wang on 18-3-3.
//

#ifndef HELLO_HISTORY2_H
#define HELLO_HISTORY2_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
using boost::gregorian::date;
using boost::gregorian::date_period;
using boost::date_time::neg_infin;
using boost::lexical_cast;
using boost::bad_lexical_cast;

using std::string;
using std::vector;
using std::shared_ptr;
using std::ostream;
using std::cout;
using std::endl;


const date_period dpnull = date_period(date(neg_infin), date(neg_infin));

// 时间段
class histbase
{
public:
    explicit histbase() : dp(dpnull) {}
    explicit histbase(date dtbegin, date dtend) : dp(dtbegin, dtend) {}
    explicit histbase(const date_period &dtprd) : dp(dtprd) {}
//    bool contains(date dt) { return dp.contains(dt); }

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
//    const T &at(const date &dt) { return dp.contains(dt) ? data : T(); }
//    T &get() { return data; }

    T data;
};

// 历史记录，一串时间段内的数据
template <class T>
class history2
{
public:
    typedef vector<histnode<T> > vtnodes_type;
    typedef histnode<T> node_type;
    typedef T base_type;

    vtnodes_type &getvtdata() { return vtdata; }

    bool insnode(const node_type &node) {   // 插入一个节点
        vtdata.push_back(node);
        return true;
    }
    base_type const *at(date dt) {          // 时间点上的数据
        for (auto it = vtdata.begin(); it != vtdata.end(); ++it)
            if (it->dp.contains(dt))
                return &(it->data);
        return nullptr;
    }
    void print() const {
        for (auto v : vtdata)
            cout << v.dp << " -> " << v.data << endl;
    }

protected:
    vtnodes_type vtdata;
};

// 数据定义
class licshiprec
{
public:
    licshiprec() = default;
    licshiprec(const licshiprec &r) = default;
    licshiprec(const date_period &dp, const string &cmpn,
    double vzd, double vzzd, const string &cont)
    : valid(true), validperiod(dp), compname(cmpn)
            , zd(vzd), zzd(vzzd), content(cont) {}

    bool valid;
    date_period validperiod = dpnull;
    string compname;
    double zd;
    double zzd;
    string content;
};
typedef history2<licshiprec> hislicship_t;

class shiprec
{
public:
    shiprec() = default;
    shiprec(const shiprec &r) = default;
    shiprec(const string &vname, double vzd, double vzzd, int vgl)
            : valid(true), name(vname), zd(vzd), zzd(vzzd), gl(vgl) {}
    shiprec(const string &sname, const string &szd, const string &szzd, const string &sgl)
            : valid(true), name(sname),
              zd(lexical_cast<double>(szd)),
              zzd(lexical_cast<double>(szzd)),
              gl(lexical_cast<int>(sgl)) {}
    shiprec(const vector<string> &vs) {
        if (vs.size() < 4)
            valid = false;
        else {
            valid = true;
            name = vs[0];
            zd = lexical_cast<double>(vs[1]);
            zzd = lexical_cast<double>(vs[2]);
            gl = lexical_cast<int>(vs[3]);
        }
    }

    void print() const {
        cout << "(" << name << ", " << zd
                  << ", " << zzd << ", " << gl << ")";
    }
    friend ostream &operator<<(ostream &os, const shiprec &r) {
        return cout << "(" << r.name << ", " << r.zd
                         << ", " << r.zzd << ", " << r.gl << ")";
    }

    bool valid;
    string name;
    double zd;
    double zzd;
    int gl;
    shared_ptr<hislicship_t> lic;
    shared_ptr<hislicship_t> hklic;
};
typedef history2<shiprec> hisship_t;


class companyrec
{
    string name;
    string addr;
    string legalman;
};



#endif //HELLO_HISTORY2_H
