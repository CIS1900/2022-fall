enum class color { red, green, blue };

int main()
{
    color c1 {color::blue};

    int n = static_cast<int>(c1); // explicitly convert to an int, we'll talk about static_cast in a later class

    color c2 {0}; // explicitly convert number to a color on construction
    // ints cannot be converted into colors after initialization
    // c2 = 1;
    // but we can use the constructor to do this
    c2 = color {1};
}
