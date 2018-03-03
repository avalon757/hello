//
// Created by wang on 18-3-3.
//

#ifndef HELLO_HISTORY2_H
#define HELLO_HISTORY2_H

#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost::gregorian;
using std::string;
using std::vector;

// 时间段
class histbase
{
    date_period dp;
    bool inthis(const date &dt) { return dp.contains(dt); }
};

// 时间段内的数据
template <class T>
class histnode : public histbase
{
    T data;
    const T &getdata(const date &dt) { return data; }
};

// 历史记录，一串时间段内的数据
template <class T>
class history2
{
    typedef vector<histnode<T> > vhist;
    vhist vtdata;
};

// 数据定义
class shipbase
{
    string name;
    double zd;
    double zzd;
    int gl;
};

class compbase
{
    string name;
    string addr;
    string legalman;
};

//class histbase
//{
//    date_period dp;
//    bool inthis(const date &dt) { return dp.contains(dt); }
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
