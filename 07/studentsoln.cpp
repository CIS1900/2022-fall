#include <string>

#include "student.hpp"

using std::string;

student::student(string name) : name {name},
                                grade {nullopt}
{
}

student::student(string name, double grade) : name {name}
{
    setGrade(grade)
}

void student::setGrade(double g)
{
    grade = g;

    if (*grade > 100)
        *grade = 100;
    if (*grade < 0)
        *grade = 0;
}
