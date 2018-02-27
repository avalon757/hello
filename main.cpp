#include <iostream>
#include <boost/timer.hpp>
using namespace std;

int main() {
    boost::timer t;
    std::cout << "Hello, World!" << std::endl;
    std::cout << "中华人民共和国" << std::endl;
    cout << t.elapsed() << endl;
    return 0;
}