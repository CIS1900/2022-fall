int main()
{
    auto * p {new int}; // auto p also works, but this is more explicit that p should be a pointer
    *p = 10;
    delete p;

    auto * q {new int[10]};
    q[5] = 9;
    delete[] q;
}
