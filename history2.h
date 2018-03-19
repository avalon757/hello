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
            : histbase(dpnull), data(make_shared<T>()) {}
    explicit histnode(const T &d, const date_period &vdp = dpnull)
            : histbase(vdp) {
        data = make_shared<T>(d);
    }
    explicit histnode(shared_ptr<T> d, const date_period &vdp = dpnull)
            : data(d), histbase(vdp) {}
//    const T &at(const date &dt) { return dp.contains(dt) ? data : T(); }
//    T &get() { return data; }

    shared_ptr<T> data;
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

    shared_ptr<T> at(const date &dt) {          // 时间点上的数据
        for (auto it = vtdata.rbegin(); it != vtdata.rend(); ++it)
            if (it->dp.contains(dt))
                return it->data;
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
            cout << v.dp << " -> " << *(v.data) << endl;
    }

protected:
    int id;
    vtnodes_type vtdata;
};

class yntype {
public:
    yntype() = default;
    yntype(const yntype &r) = default;
    yntype(unsigned short itype) {
        setyntype(itype);
    }
    yntype(const string &stype) {
        setyntype(stype);
    }

    void setyntype(unsigned short itype) {
        ynssn = ((itype & 0x1) != 0);
        ynssj = ((itype & 0x2) != 0);
        ynsn  = ((itype & 0x4) != 0);
        ynsj  = ((itype & 0x8) != 0);
        ynjy  = ((itype & 0x10) != 0);
        yngo  = ((itype & 0x20) != 0);
    }
    void setyntype(const string &stype) {
        ynssn = (stype[5] == '1');
        ynssj = (stype[4] == '1');
        ynsn  = (stype[3] == '1');
        ynsj  = (stype[2] == '1');
        ynjy  = (stype[1] == '1');
        yngo  = (stype[0] == '1');
    }
    unsigned short getyntype() {
        unsigned short ret = 0x0;
        if (ynssn) ret |= 0x1;
        if (ynssj) ret |= 0x2;
        if (ynsn ) ret |= 0x4;
        if (ynsj ) ret |= 0x8;
        if (ynjy ) ret |= 0x10;
        if (yngo ) ret |= 0x20;
        return ret;
    }

    friend ostream &operator<<(ostream &os, const yntype& yn) {
        return os << yn.yngo << "|" << yn.ynjy << "|" << yn.ynsj
                  << "|" << yn.ynsn  << "|" << yn.ynssj << "|" << yn.ynssn;
    }
    // go|jy||sj|sn||ssj|ssn  [6 - 0]
    bool yngo;
    bool ynjy;
    bool ynsj;
    bool ynsn;
    bool ynssj;
    bool ynssn;
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
        yn.setyntype(typ);
    }
    unsigned short getyntype() {
        return yn.getyntype();
    }

    friend ostream &operator<<(ostream &os, const licshiprec &r) {
        return os << "(" << r.validperiod << ", " << r.licno << ", " << r.shipname
                << ", " << r.compname << ", " << r.content << ", " << r.yn << ")";
    }

    bool valid;
    date_period validperiod = dpnull;
    string licno;
    string shipname;
    string compname;
    string content;
    yntype yn;
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
        yn.setyntype(typ);
    }
    unsigned short getyntype() {
        return yn.getyntype();
    }

    friend ostream &operator<<(ostream &os, const liccomprec &r) {
        return os << "(" << r.validperiod << ", " << r.licno << ", " << r.compname
                    << ", " << r.content << ", " << r.yn << ")";
    }

    bool valid;
    date_period validperiod = dpnull;
    string licno;
    string compname;
    string content;
    yntype yn;
};
typedef history2<liccomprec> hisliccomp_t;

class companyrec
{
public:
    companyrec() = default;
    companyrec(const companyrec &r) = default;
    companyrec(const string &na, const string &ad, const string &lem, double rc, date opt, unsigned short ar)
            : name(na), addr(ad), legalman(lem), regcapital(rc), opentime(opt), areaid(ar) {}
    companyrec(const string &na, const string &ad, const string &lem, const string &src, const string &sopt, const string &sar)
            : name(na), addr(ad), legalman(lem),
              regcapital(lexical_cast<double>(src)),
              opentime(from_string(sopt)), areaid(lexical_cast<unsigned short>(sar)) {}
    companyrec(const vector<string> &vs)
    {
        if (vs.size() < 6) return;
        name = vs[0];
        addr = vs[1];
        legalman = vs[2];
        regcapital = (vs[3].empty() ? 0 : lexical_cast<double>(vs[3]));
        opentime = (vs[4].empty() ? date(1970,1,1) : from_string(vs[4]));
        areaid = lexical_cast<unsigned short>(vs[5]);
    }

    friend ostream &operator<<(ostream &os, const companyrec &r) {
        return os << "(" << r.name << ", " << r.addr << ", " << r.legalman
                  << ", " << r.regcapital << ", " << r.opentime << ", " << r.areaid << ")";
    }

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
    shiprec(const string &vname, double vzd, double vzzd, int vteu, double vgl, unsigned short itype)
            : valid(true), name(vname), zd(vzd), zzd(vzzd), teu(vteu), gl(vgl), yn(itype) {}
    shiprec(const string &sname, const string &szd, const string &szzd
            , const string &steu, const string &sgl, const string &stype = "000000")
            : valid(true), name(sname),
              zd(lexical_cast<double>(szd)),
              zzd(lexical_cast<double>(szzd)),
              teu(lexical_cast<int>(steu)),
              gl(lexical_cast<double>(sgl)),
              yn(stype) {}
    shiprec(const vector<string> &vs) {
        if (vs.size() < 6)
            valid = false;
        else {
            valid = true;
            name = vs[0];
            zd = (vs[1].empty() ? 0 : lexical_cast<double>(vs[1]));
            zzd = (vs[2].empty() ? 0 : lexical_cast<double>(vs[2]));
            teu = (vs[3].empty() ? 0 : lexical_cast<int>(vs[3]));
            gl = (vs[4].empty() ? 0 : lexical_cast<double>(vs[4]));
            yn.setyntype(vs[5]);
        }
    }
    void setyntype(unsigned short itype) {
        yn.setyntype(itype);
    }
    void setyntype(const string &stype) {
        yn.setyntype(stype);
    }
    unsigned short getyntype() {
        return yn.getyntype();
    }

    void print() const {
        cout << "(" << name << ", " << zd << ", " << zzd
             << ", " << teu << ", " << gl << ", " <<  yn << ")";
    }
    friend ostream &operator<<(ostream &os, const shiprec &r) {
        return os << "(" << r.name << ", " << r.zd << ", " << r.zzd
                  << ", " << r.teu << ", " << r.gl <<  ", " << r.yn << ")";
    }

    bool valid;
    string name;
    double zd;
    double zzd;
    int teu;
    double gl;
    yntype yn;
    shared_ptr<hisliccomp_t> cmpy;
    shared_ptr<hislicship_t> lic;
    shared_ptr<hislicship_t> hklic;
};
typedef history2<shiprec> hisship_t;




#endif //HELLO_HISTORY2_H
