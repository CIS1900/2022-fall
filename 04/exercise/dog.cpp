#include "dog.hpp"
#include <iostream>

dog::dog(std::string name) : animal {name}
{}

// const is part of the type, but override is not (it's more of a flag to the compiler to check that it overrides something), so that's why we have const here but not override
void dog::pet() const
{
    std::cout << name << " wags its tail.";
}
