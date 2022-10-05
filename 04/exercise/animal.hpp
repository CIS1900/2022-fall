#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <string>

class animal
{
protected:
    std::string name;
public:
    animal(std::string);

    virtual void pet() const = 0;
};

#endif
