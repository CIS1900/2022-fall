#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    ostringstream oss;
    oss << 1.1 << " " << scientific << 1.1;
    cout << oss.str() << "\n";

    istringstream iss {oss.str()};
    double a {0}, b {0};
    iss >> a >> b;
    cout << a << " " << b;
}
