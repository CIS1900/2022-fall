#ifndef DOG_HPP
#define DOG_HPP

#include "animal.hpp"

class dog : public animal
{
public:
    dog(std::string);

    void pet() const override;
};

#endif
