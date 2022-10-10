#include <iostream>
#include <memory>

class list
{
public:
    // left the members public for convenience
    int val;
    list * next;

    list(int val) : val {val}, next {nullptr}
    {}

    list(const list & other) = delete;

    list(list && other) : val {other.val}, next {other.next}
    {}

    ~list()
    {
        // note that we are deleting a list pointer, so its destructor is called as well automatically, recursively deallocating the entire linked list
        delete next;
    }

    list & operator=(const list & other) = delete;

    list & operator=(list && other)
    {
        val = other.val;
        next = other.next;
        other.next = nullptr;

        return *this;
    }

    void print() const
    {
        std::cout << val << " ";
        if (next)
            next->print();
    }
};

int main()
{
}
