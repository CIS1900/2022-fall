#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    const string filename {"test.txt"};

    {
        ofstream ofs {filename};
        if (!ofs)
            cerr << "Error opening file"; // the file is possibly open in another process
        ofs << "test";
    } // ofs goes out of scope, so it is cleaned up and the file is written to here. We could also have called ofs.close();

    ifstream ifs {filename};
    if (!ifs)
        cerr << "Error opening file"; // the file may not exist

    if (string s; ifs >> s)
        cout << s;
    else
        cerr << "Nothing to read";
}
