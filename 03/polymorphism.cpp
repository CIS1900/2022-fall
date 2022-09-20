#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void log(ostream & os)
{
    os << "Logging\n";
}

int main()
{
    ofstream ofs {"test.log"};
    ostringstream oss;
    oss << "Log file start:\n";

    // the if statement condition could be changed so the other branches execute
    if (false)
        log(cout);
    else if (false)
        log(ofs);
    else
    {
        log(oss);
        cout << oss.str();
    }
}
