#include <iostream>
#include <unordered_set>

using namespace std;

struct S
{
    string first_name;
    string last_name;

    bool operator==(const S& other) const
    {
        return first_name == other.first_name && last_name == other.last_name;
    }
};

// Writing our own functor to pass to an unordered_set
struct myHash
{
    size_t operator()(S const& s) const noexcept
    {
        size_t h1 {hash<string>{}(s.first_name)};
        size_t h2 {hash<string>{}(s.last_name)};
        return h1 ^ (h2 << 1);
    }
};


int main()
{
    unordered_set<S, myHash> s2 { {"Paul", "He"} };
}
