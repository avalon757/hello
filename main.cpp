#include <iostream>
#include <boost/timer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace std;
using namespace boost::gregorian;

extern void test_history2();
extern void test_history1();

int main() {

    boost::timer t;

    test_history1();
    test_history2();

    cout << t.elapsed() << endl;

    return 0;
}