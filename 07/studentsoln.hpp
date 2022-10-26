#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <optional>

class student
{
private:
    std::string name;
    std::optional<double> grade;
public:
    student(std::string);
    student(std::string, double);

    student(student &&) = default;
    student & operator=(student &&) = default;

    void setGrade(double);

    // assume there is some function to write the student data to some custom format
};

#endif
