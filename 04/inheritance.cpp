#include <iostream>

using std::cout;
using std::string;

class animal
{
protected:
    string name;

public:
    animal(string name) : name {name}
    {}

    virtual void pet() const = 0;
};

class dog : public animal
{
public:
    dog(string name) : animal {name}
    {}

    void pet() const override
    {
        cout << name << " wags its tail.";
    }
};

class cat : public animal
{
public:
    cat(string name) : animal {name}
    {}

    void pet() const override
    {
        cout << name << " purrs.";
    }
};

void petLots(const animal & a)
{
    for (int i {0}; i < 10; ++i)
    {
        a.pet();
        cout << "\n";
    }
}

int main()
{
    dog d {"Rover"};
    cat c {"Strawberry"};

    petLots(d);
    petLots(c);
}
