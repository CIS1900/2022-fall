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
    {
        other.next = nullptr;
    }

    ~list()
    {
        // note that we are deleting a list pointer, so its destructor is called as well automatically, recursively deallocating the entire linked list
        delete next;
    }

    list & operator=(const list & other) = delete;

    list & operator=(list && other)
    {
        if (this == &other)
            return *this;

        val = other.val;

        delete next;

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
    list l1 {1};
    l1.next = new list {2};

    list l2 {3};
    l2.next = new list {4};

    l2 = std::move(l2);

    l2.print();
}
