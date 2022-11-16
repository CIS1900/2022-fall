#ifndef UNIQUE_PTR_HPP
#define UNIQUE_PTR_HPP

template <typename T>
class unique_ptr
{
    T * p;
public:
    unique_ptr(T * p) noexcept : p {p}
    {}

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr(unique_ptr && o) noexcept : p {o.p}
    {
        o.p = nullptr;
    }

    ~unique_ptr()
    {
        delete p;
    }

    unique_ptr & operator=(const unique_ptr&) = delete;

    unique_ptr & operator=(unique_ptr && o) noexcept
    {
        delete p;
        p = o.p;
        o.p = nullptr;
    }

    // TODO
    T * release() noexcept
    {
        T * q {p};
        p = nullptr;
        return q;
    }

    void reset(T * q) noexcept
    {
        delete p;
        p = q;
    }

    T * get() const noexcept
    {
        return p;
    }

    T & operator*() const
    {
        return *p;
    }

    T * operator->() const noexcept
    {
        return p;
    }
};

#endif
