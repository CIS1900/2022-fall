#include <string>
#include <memory>

class student
{
private:
    std::string name;
    std::unique_ptr<double> grade;
public:
    student(std::string);
    student(std::string, double);

    student(const student &);
    student(student &&) = default;

    student & operator=(const student &);
    student & operator=(student &&) = default;

    void setGrade(double);

    // assume there is some function to write the student data to some custom format
};
