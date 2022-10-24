#include <string>
#include <memory>

#include "student.hpp"

using namespace std;

student::student(string name) : name {name},
                                grade {nullptr}
{
}

student::student(string name, double grade) : name {name},
                                              grade {make_unique<double>(grade)}
{
    if (*this->grade > 100)
        *this->grade = 100;
    if (*this->grade < 0)
        *this->grade = 0;
}

student::student(const student & other) :
    name {other.name},
    grade {other.grade ? make_unique<double>(*other.grade) : nullptr}
{
}

student & student::operator=(const student & other)
{
    name = other.name;
    if (grade)
    {
        if (other.grade)
            *grade = *other.grade;
        else
            grade = nullptr;
    }
    else
    {
        if (other.grade)
            grade = make_unique<double>(*other.grade);
    }

    return *this;
}

void student::setGrade(double g)
{
    if (grade)
        *grade = g;
    else
        grade = make_unique<double>(g);

    if (*grade > 100)
        *grade = 100;
    if (*grade < 0)
        *grade = 0;
}
