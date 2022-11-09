#ifndef WRAPPER_HPP
#define WRAPPER_HPP

template <typename T>
class wrapper
{
    T t;
public:
    wrapper(T t);

    T get() const;
    void set(T t);
};

#include "wrapper.tpp"

#endif
