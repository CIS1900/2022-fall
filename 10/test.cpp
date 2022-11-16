#include "unique_ptr.hpp"

struct foo
{
    int i;
};

int main()
{
    unique_ptr<foo> p {new foo};

    p->i = 10;
}
