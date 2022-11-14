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
        p = o.p;
        o.p = nullptr;
    }

    // TODO
    T * release() noexcept;
    void reset(T *) noexcept;
    T * get() const noexcept;

    T & operator*() const;
    T * operator->() const noexcept;
};

#endif
