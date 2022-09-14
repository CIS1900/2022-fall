#include <iostream>

void div(const int a, const int b, int & q, int & r)
{
    if (b == 0)
        return;

    q = a / b;
    r = a % b;
}

int main()
{
    auto q {0};
    auto r {0};

    div(9, 2, q, r);

    std::cout << "9 / 2 = " << q << " remainder " << r << "\n";
}
