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

    list(const list & other) :
        val {other.val},
        next {other.next ? new list {*other.next} : nullptr}
    {}

    ~list()
    {
        // note that we are deleting a list pointer, so its destructor is called as well automatically, recursively deallocating the entire linked list
        delete next;
    }

    list & operator=(const list & other)
    {
        val = other.val;
        next = other.next;

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
