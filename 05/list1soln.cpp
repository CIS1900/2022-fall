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
        if (this == &other)
            return *this;

        val = other.val;

        delete next;
        if (other.next)
            next = new list {*other.next};
        else
            next = nullptr;

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
    list l1 {1};
    l1.next = new list {2};

    list l2 {2};
    l2.next = new list {3};

    l2 = l2;

    l2.print();
}
