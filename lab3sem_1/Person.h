#pragma once

#include <iostream>
#include <string>
#include <stdexcept>

struct PersonID {
    int series; //серия
    int number; //номер
};

class Person {
protected:
    PersonID id; //сразк вся инф
    std::string firstName;
    std::string middleName;
    std::string lastName;
    int birthYear; //год рождения

public:
    Person() {
        this->id.series = 0;
        this->id.number = 0;
        this->firstName = ""; //имя
        this->middleName = ""; //отчество
        this->lastName = ""; //фамилия
        this->birthYear = 0;
    }

    Person(int series, int number, const std::string& lastName, const std::string& firstName, //конструктор
           const std::string& middleName, int birthYear) {
        SetPerson(series, number, lastName, firstName, middleName, birthYear);
    }

    void SetPerson(int series, int number, const std::string& lastName, const std::string& firstName,//просто метод
                   const std::string& middleName, int birthYear) {
        if (series < 0 or number < 0) {
            throw std::invalid_argument("Person: series and number must be non-negative");
        }
        this->id.series = series;
        this->id.number = number;
        this->lastName = lastName;
        this->firstName = firstName;
        this->middleName = middleName;
        this->birthYear = birthYear;
    }

    PersonID GetID() const { //получить серию и номер
        return this->id;
    }

    std::string GetFirstName() const { //получить имя
        return this->firstName;
    }

    std::string GetMiddleName() const { //получить отчество
        return this->middleName;
    }

    std::string GetLastName() const { //получить фамилию
        return this->lastName;
    }

    int GetBirthYear() const { //получить год рождения
        return this->birthYear;
    }

    std::string GetFullName() const { // фамилия имя отчество
        return this->lastName + " " + this->firstName + " " + this->middleName;
    }

    bool operator<(const Person& other) const { //меньше ли серия с номером
        if (this->id.series != other.id.series) {
            return this->id.series < other.id.series;
        }
        return this->id.number < other.id.number;
    }

    bool operator==(const Person& other) const { // равны ли данные
        return this->id.series == other.id.series and this->id.number == other.id.number;
    }
};

class Student : public Person {
private:
    std::string group; // номер группы

public:
    Student() { // пустая группа
        this->group = "";
    }

    Student(int series, int number, const std::string& lastName, const std::string& firstName, //добавим группу
            const std::string& middleName, int birthYear, const std::string& group) {
        SetPerson(series, number, lastName, firstName, middleName, birthYear);
        this->group = group;
    }

    std::string GetGroup() const { //получить группу
        return this->group;
    }
};

class Teacher : public Person {
private:
    std::string department; //кафедра

public:
    Teacher() {
        this->department = ""; //пустая кафедра
    }

    Teacher(int series, int number, const std::string& lastName, const std::string& firstName, //заполняем кафедру
            const std::string& middleName, int birthYear, const std::string& department) {
        SetPerson(series, number, lastName, firstName, middleName, birthYear);
        this->department = department;
    }

    std::string GetDepartment() const {//получить кафедру
        return this->department;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Student& student) { //вывод через пробел
    out << student.GetID().series << " " << student.GetID().number << " "
        << student.GetLastName() << " " << student.GetFirstName() << " "
        << student.GetMiddleName() << " " << student.GetBirthYear() << " "
        << student.GetGroup();
    return out;
}

inline std::istream& operator>>(std::istream& in, Student& student) {
    int series = 0;
    int number = 0;
    std::string lastName;
    std::string firstName;
    std::string middleName;
    int birthYear = 0;
    std::string group;
    in >> series >> number >> lastName >> firstName >> middleName >> birthYear >> group;
    if (!in or series < 0 or number < 0) {
        in.setstate(std::ios::failbit); //поток сломан, выйти
        return in;
    }
    student = Student(series, number, lastName, firstName, middleName, birthYear, group);
    return in;
}

inline std::ostream& operator<<(std::ostream& out, const Teacher& teacher) {//то же самое для учителя
    out << teacher.GetID().series << " " << teacher.GetID().number << " "
        << teacher.GetLastName() << " " << teacher.GetFirstName() << " "
        << teacher.GetMiddleName() << " " << teacher.GetBirthYear() << " "
        << teacher.GetDepartment();
    return out;
}

inline std::istream& operator>>(std::istream& in, Teacher& teacher) { //из потока в переменные
    int series = 0;
    int number = 0;
    std::string lastName;
    std::string firstName;
    std::string middleName;
    int birthYear = 0;
    std::string department;
    in >> series >> number >> lastName >> firstName >> middleName >> birthYear >> department;
    if (!in or series < 0 or number < 0) {
        in.setstate(std::ios::failbit);
        return in;
    }
    teacher = Teacher(series, number, lastName, firstName, middleName, birthYear, department);
    return in;
}
