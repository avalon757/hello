//
// Created by wang on 18-3-3.
//

#ifndef HELLO_HISTORY2_H
#define HELLO_HISTORY2_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/lexical_cast.hpp>
using boost::gregorian::date;
using boost::gregorian::date_period;
using boost::gregorian::from_string;
using boost::date_time::neg_infin;
using boost::lexical_cast;
using boost::bad_lexical_cast;

#include <algorithm>
using std::sort;
using std::unique;

using std::string;
using std::vector;
using std::shared_ptr;
using std::make_shared;
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
    typedef T base_type;
    typedef histnode<T> node_type;
    typedef vector<node_type> vtnodes_type;

    int getid() { return id; }
    vtnodes_type &getvtdata() { return vtdata; }

    bool insnode(const node_type &node) {   // 插入一个节点
        if (!vtdata.empty()) {  // 每次插入时，截掉最后节点尾部超出期限
            node_type &lastnode = vtdata.back();
            if (lastnode.dp.end() > node.dp.begin())
                lastnode.dp = date_period(lastnode.dp.begin(), node.dp.begin());
        }
        vtdata.push_back(node);
        return true;
    }

    base_type const *at(date dt) {          // 时间点上的数据
        for (auto it = vtdata.rbegin(); it != vtdata.rend(); ++it)
            if (it->dp.contains(dt))
                return &(it->data);
        return nullptr;
    }

    shared_ptr<vector<date>> dateserial() {
        shared_ptr<vector<date>> p = make_shared<vector<date>>();
        for (auto v : vtdata) {
            p->push_back(v.dp.begin());
            p->push_back(v.dp.end());
        }
        sort(p->begin(), p->end());
        auto item = unique(p->begin(), p->end());
        p->erase(item, p->end());
        return p;
    }

    void print() const {
        for (auto v : vtdata)
            cout << v.dp << " -> " << v.data << endl;
    }

protected:
    int id;
    vtnodes_type vtdata;
};

// 数据定义
class licshiprec {
public:
    licshiprec() = default;
    licshiprec(const licshiprec &r) = default;
    licshiprec(const date_period &dp, const string &lcn, const string &shpn, const string &cmpn,
               const string &cont, unsigned char typ)
            : valid(true), validperiod(dp), licno(lcn), shipname(shpn), compname(cmpn), content(cont) {
        setyntype(typ);
    }
    licshiprec(const string &sdbeg, const string &sdend, const string &slcn, const string &sshpn,
               const string &scmpn, const string &scont, const string &styp)
            : valid(true), validperiod(date_period(from_string(sdbeg), from_string(sdend))),
              licno(slcn), shipname(sshpn), compname(scmpn), content(scont) {
        setyntype(lexical_cast<unsigned short>(styp));
    }
    void setyntype(unsigned short typ) {
        ynssn = ((typ & 0x1) != 0);
        ynssj = ((typ & 0x2) != 0);
        ynsn  = ((typ & 0x4) != 0);
        ynsj  = ((typ & 0x8) != 0);
        ynjy  = ((typ & 0x10) != 0);
        yngo  = ((typ & 0x20) != 0);
    }
    unsigned short getyntype() {
        unsigned char ret = 0x0;
        if (ynssn) ret |= 0x1;
        if (ynssj) ret |= 0x2;
        if (ynsn ) ret |= 0x4;
        if (ynsj ) ret |= 0x8;
        if (ynjy ) ret |= 0x10;
        if (yngo ) ret |= 0x20;
        return ret;
    }

    friend ostream &operator<<(ostream &os, const licshiprec &r) {
        return os << "(" << r.validperiod << ", " << r.licno << ", " << r.shipname
                << ", " << r.compname << ", " << r.content << ", " << r.yngo << "|" << r.ynjy
                << "|" << r.ynsj << "|" << r.ynsn<< "|" << r.ynssj << "|" << r.ynssn << ")";
    }

    bool valid;
    date_period validperiod = dpnull;
    string licno;
    string shipname;
    string compname;
    string content;
    // go|jy||sj|sn||ssj|ssn  [6 - 0]
    bool yngo;
    bool ynjy;
    bool ynsj;
    bool ynsn;
    bool ynssj;
    bool ynssn;
};
typedef history2<licshiprec> hislicship_t;

class liccomprec {
public:
    liccomprec() = default;
    liccomprec(const liccomprec &r) = default;
    liccomprec(const date_period &dp, const string &lcn, const string &cmpn,
               const string &cont, unsigned char typ)
            : valid(true), validperiod(dp), licno(lcn), compname(cmpn), content(cont) {
        setyntype(typ);
    }
    liccomprec(const string &sdbeg, const string &sdend, const string &slcn, const string &scmpn,
               const string &scont, const string &styp)
            : valid(true), validperiod(date_period(from_string(sdbeg), from_string(sdend))),
              licno(slcn), compname(scmpn), content(scont) {
        setyntype(lexical_cast<unsigned short>(styp));
    }
    void setyntype(unsigned short typ) {
        ynssn = ((typ & 0x1) != 0);
        ynssj = ((typ & 0x2) != 0);
        ynsn  = ((typ & 0x4) != 0);
        ynsj  = ((typ & 0x8) != 0);
        ynjy  = ((typ & 0x10) != 0);
        yngo  = ((typ & 0x20) != 0);
    }
    unsigned short getyntype() {
        unsigned char ret = 0x0;
        if (ynssn) ret |= 0x1;
        if (ynssj) ret |= 0x2;
        if (ynsn ) ret |= 0x4;
        if (ynsj ) ret |= 0x8;
        if (ynjy ) ret |= 0x10;
        if (yngo ) ret |= 0x20;
        return ret;
    }

    friend ostream &operator<<(ostream &os, const liccomprec &r) {
        return os << "(" << r.validperiod << ", " << r.licno << ", " << r.compname
                    << ", " << r.content << ", " << r.yngo << "|" << r.ynjy
                << "|" << r.ynsj << "|" << r.ynsn<< "|" << r.ynssj << "|" << r.ynssn << ")";
    }

    bool valid;
    date_period validperiod = dpnull;
    string licno;
    string compname;
    string content;
    // go|jy||sj|sn||ssj|ssn  [6 - 0]
    bool yngo;
    bool ynjy;
    bool ynsj;
    bool ynsn;
    bool ynssj;
    bool ynssn;
};
typedef history2<liccomprec> hisliccomp_t;

class companyrec
{
    companyrec() = default;
    companyrec(const companyrec &r) = default;
    companyrec(const string &na, const string &ad, const string &lem, double rc, date opt, unsigned short ar)
            : name(na), addr(ad), legalman(lem), regcapital(rc), opentime(opt), areaid(ar) {}
    companyrec(const string &na, const string &ad, const string &lem, const string &src, const string &sopt, const string &sar)
            : name(na), addr(ad), legalman(lem),
              regcapital(lexical_cast<double>(src)),
              opentime(from_string(sopt)), areaid(lexical_cast<unsigned short>(sar)) {}
    string name;
    string addr;
    string legalman;
    double regcapital;
    date opentime;
    unsigned short areaid;
    shared_ptr<hisliccomp_t> lic;
    shared_ptr<hisliccomp_t> hklic;
};
typedef history2<companyrec> hiscomp_t;

class shiprec
{
public:
    shiprec() = default;
    shiprec(const shiprec &r) = default;
    shiprec(const string &vname, double vzd, double vzzd, int vteu, int vgl)
            : valid(true), name(vname), zd(vzd), zzd(vzzd), teu(vteu), gl(vgl) {}
    shiprec(const string &sname, const string &szd, const string &szzd, const string &steu, const string &sgl)
            : valid(true), name(sname),
              zd(lexical_cast<double>(szd)),
              zzd(lexical_cast<double>(szzd)),
              teu(lexical_cast<int>(steu)),
              gl(lexical_cast<int>(sgl)) {}
    shiprec(const vector<string> &vs) {
        if (vs.size() < 4)
            valid = false;
        else {
            valid = true;
            name = vs[0];
            zd = lexical_cast<double>(vs[1]);
            zzd = lexical_cast<double>(vs[2]);
            teu = lexical_cast<int>(vs[3]);
            gl = lexical_cast<int>(vs[4]);
        }
    }

    void print() const {
        cout << "(" << name << ", " << zd
                  << ", " << zzd << ", " << teu << ", " << gl << ")";
    }
    friend ostream &operator<<(ostream &os, const shiprec &r) {
        return os << "(" << r.name << ", " << r.zd
                         << ", " << r.zzd << ", " << r.teu << ", " << r.gl << ")";
    }

    bool valid;
    string name;
    double zd;
    double zzd;
    int teu;
    int gl;
    shared_ptr<hisliccomp_t> cmpy;
    shared_ptr<hislicship_t> lic;
    shared_ptr<hislicship_t> hklic;
};
typedef history2<shiprec> hisship_t;




#endif //HELLO_HISTORY2_H
