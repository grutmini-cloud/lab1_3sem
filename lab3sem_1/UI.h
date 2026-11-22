#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "BinaryTree.h"
#include "Complex.h"
#include "Person.h"
#include "Tests.h"
#include "Benchmark.h"

using namespace std;

template <typename T>
bool Read(T& value) { // прочитать значение, при ошибке очистить ввод
    if (cin >> value) {
        return true;
    }
    cin.clear();
    cin.ignore(10000, '\n');
    cout << "Wrong input\n";
    return false;
}

string ReadLine() { // прочитать целую строку
    string line;
    cin >> ws;
    getline(cin, line);
    return line;
}

template <typename T>
bool ReadKey(T& key) { // ключ для поиска/удаления: само значение
    cout << "Value: ";
    return Read(key);
}

bool ReadKey(Student& key) { // ключ для студента: только паспорт
    int series;
    int number;
    cout << "Passport series and number: ";
    if (!Read(series) or !Read(number)) {
        return false;
    }
    key.SetPerson(series, number, "", "", "", 0);
    return true;
}

bool ReadKey(Teacher& key) { // ключ для преподавателя: только паспорт
    int series;
    int number;
    cout << "Passport series and number: ";
    if (!Read(series) or !Read(number)) {
        return false;
    }
    key.SetPerson(series, number, "", "", "", 0);
    return true;
}

int MapFunction(int x) { // x * 2
    return x * 2;
}

double MapFunction(double x) { // x * 2
    return x * 2;
}

Complex MapFunction(Complex x) { // x * i
    return x * Complex(0, 1);
}

string MapFunction(string x) { // удвоить строку
    return x + x;
}

Student MapFunction(Student s) { // год рождения + 1
    return Student(s.GetID().series, s.GetID().number, s.GetLastName(), s.GetFirstName(),
                   s.GetMiddleName(), s.GetBirthYear() + 1, s.GetGroup());
}

Teacher MapFunction(Teacher t) { // год рождения + 1
    return Teacher(t.GetID().series, t.GetID().number, t.GetLastName(), t.GetFirstName(),
                   t.GetMiddleName(), t.GetBirthYear() + 1, t.GetDepartment());
}

bool WhereFunction(int x) { // чётные
    return x % 2 == 0;
}

bool WhereFunction(double x) { // положительные
    return x > 0;
}

bool WhereFunction(Complex x) { // модуль больше 2
    return x.Abs() > 2;
}

bool WhereFunction(string x) { // длиннее 3 символов
    return x.size() > 3;
}

bool WhereFunction(Student s) { // родились в 2005 или позже
    return s.GetBirthYear() >= 2005;
}

bool WhereFunction(Teacher t) { // родились в 1975 или позже
    return t.GetBirthYear() >= 1975;
}

int ReduceFunction(int item, int result) { // сумма
    return item + result;
}

double ReduceFunction(double item, double result) { // сумма
    return item + result;
}

Complex ReduceFunction(Complex item, Complex result) { // сумма
    return item + result;
}

string ReduceFunction(string item, string result) { // склеить
    return result + item;
}

Student ReduceFunction(Student item, Student result) { // самый старший
    if (item.GetBirthYear() < result.GetBirthYear()) {
        return item;
    }
    return result;
}

Teacher ReduceFunction(Teacher item, Teacher result) { // самый старший
    if (item.GetBirthYear() < result.GetBirthYear()) {
        return item;
    }
    return result;
}

template <typename T>
T ReduceStart(const BinaryTree<T>&) { // начало reduce для чисел и строк: 0 или ""
    return T();
}

Student ReduceStart(const BinaryTree<Student>& tree) { // начало reduce для студентов: первый студент
    return tree.Get(0);
}

Teacher ReduceStart(const BinaryTree<Teacher>& tree) { // начало reduce для преподавателей: первый
    return tree.Get(0);
}

string OperationsInfo(int) { // что делают map/where/reduce для int
    return "map: x*2, where: even, reduce: sum";
}

string OperationsInfo(double) { // что делают map/where/reduce для double
    return "map: x*2, where: positive, reduce: sum";
}

string OperationsInfo(Complex) { // что делают map/where/reduce для complex
    return "map: x*i, where: |x| > 2, reduce: sum";
}

string OperationsInfo(string) { // что делают map/where/reduce для строк
    return "map: double the word, where: length > 3, reduce: concat";
}

string OperationsInfo(Student) { // что делают map/where/reduce для студентов
    return "map: year+1, where: born >= 2005, reduce: oldest";
}

string OperationsInfo(Teacher) { // что делают map/where/reduce для преподавателей
    return "map: year+1, where: born >= 1975, reduce: oldest";
}

int RandomValue(int) { // случайный int
    return rand() % 1000;
}

double RandomValue(double) { // случайный double
    return (rand() % 10000) / 10.0;
}

Complex RandomValue(Complex) { // случайное комплексное
    return Complex(rand() % 21 - 10, rand() % 21 - 10);
}

string RandomValue(string) { // случайное слово из 4 букв
    string word = "";
    for (int i = 0; i < 4; i++) {
        word += (char)('a' + rand() % 26);
    }
    return word;
}

Student RandomValue(Student) { // случайный студент
    string names[5] = {"Ivanov", "Petrov", "Sidorov", "Smirnov", "Kuznetsov"};
    return Student(4500 + rand() % 20, rand() % 1000000, names[rand() % 5], "Ivan", "Ivanovich",
                   2002 + rand() % 6, "B25-50" + to_string(rand() % 10));
}

Teacher RandomValue(Teacher) { // случайный преподаватель
    string names[5] = {"Orlova", "Volkov", "Sokolova", "Lebedev", "Kozlova"};
    return Teacher(4000 + rand() % 20, rand() % 1000000, names[rand() % 5], "Anna", "Petrovna",
                   1960 + rand() % 30, "Kafedra" + to_string(rand() % 50));
}

template <typename T>
void ShowTree(const BinaryTree<T>& tree) { // показать дерево
    cout << "Count: " << tree.GetCount() << ", height: " << tree.GetHeight() << endl;
    if (tree.IsEmpty()) {
        cout << "(empty)\n";
        return;
    }
    if (tree.GetCount() <= 20) {
        cout << "Structure: " << tree.ToFormatString("{K}(L)[P]") << endl;
    }
    cout << "Sorted:\n";
    TreeEnumerator<T> it = tree.GetEnumerator();
    int index = 0;
    while (it.MoveNext() and index < 20) {
        cout << "  " << index << ") " << it.GetCurrent() << endl;
        index++;
    }
    if (tree.GetCount() > 20) {
        cout << "  ...\n";
    }
}

template <typename T>
void ShowTreeMenu(const string& typeName) { // меню дерева типа T
    BinaryTree<T> tree;
    int choice = -1;

    while (choice != 0) {
        cout << "\n===== TREE: " << typeName << " =====\n";
        cout << "(" << OperationsInfo(T()) << ")\n";
        cout << "1. Insert\n";
        cout << "2. Remove\n";
        cout << "3. Find\n";
        cout << "4. Show tree\n";
        cout << "5. Fill with random values\n";
        cout << "6. Get by index\n";
        cout << "7. Map\n";
        cout << "8. Where\n";
        cout << "9. Reduce\n";
        cout << "10. To string\n";
        cout << "11. To format string\n";
        cout << "12. From string\n";
        cout << "13. Extract subtree\n";
        cout << "14. Clear\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        if (!Read(choice)) {
            choice = -1;
            continue;
        }

        try {
            if (choice == 1) {
                T value;
                cout << "Value: ";
                if (Read(value)) {
                    if (tree.Insert(value)) {
                        cout << "Inserted\n";
                    } else {
                        cout << "Already exists\n";
                    }
                }
            }

            else if (choice == 2) {
                T key;
                if (ReadKey(key)) {
                    tree.Remove(key);
                    cout << "Removed\n";
                }
            }

            else if (choice == 3) {
                T key;
                if (ReadKey(key)) {
                    if (tree.Contains(key)) {
                        cout << "Found: " << tree.Find(key) << endl;
                    } else {
                        cout << "Not found\n";
                    }
                }
            }

            else if (choice == 4) {
                ShowTree(tree);
            }

            else if (choice == 5) {
                int count;
                cout << "How many: ";
                if (Read(count)) {
                    for (int i = 0; i < count; i++) {
                        tree.Insert(RandomValue(T()));
                    }
                    ShowTree(tree);
                }
            }

            else if (choice == 6) {
                int index;
                cout << "Index: ";
                if (Read(index)) {
                    cout << "Element = " << tree.Get(index) << endl;
                }
            }

            else if (choice == 7) {
                cout << "Before:\n";
                ShowTree(tree);
                cout << "After map:\n";
                ShowTree(tree.Map(MapFunction));
            }

            else if (choice == 8) {
                cout << "Before:\n";
                ShowTree(tree);
                cout << "After where:\n";
                ShowTree(tree.Where(WhereFunction));
            }

            else if (choice == 9) {
                if (tree.IsEmpty()) {
                    cout << "Tree is empty\n";
                } else {
                    cout << "Reduce = " << tree.Reduce(ReduceFunction, ReduceStart(tree)) << endl;
                }
            }

            else if (choice == 10) {
                cout << tree.ToString() << endl;
            }

            else if (choice == 11) {
                cout << "Format, for example {K}(L)[P]: ";
                string format = ReadLine();
                cout << tree.ToFormatString(format) << endl;
            }

            else if (choice == 12) {
                cout << "Elements separated by spaces: ";
                string line = ReadLine();
                tree.FromString(line);
                ShowTree(tree);
            }

            else if (choice == 13) {
                T key;
                if (ReadKey(key)) {
                    BinaryTree<T> sub = tree.ExtractSubtree(key);
                    ShowTree(sub);
                    cout << "Contains this subtree: " << (tree.ContainsSubtree(sub) ? "yes" : "no") << endl;
                }
            }

            else if (choice == 14) {
                tree.Clear();
                cout << "Cleared\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

void RunUI() { // главное меню
    srand((unsigned int)time(nullptr));
    int choice = -1;

    while (choice != 0) {
        cout << "\n========== LAB 3: AVL TREE ==========\n";
        cout << "1. Run tests\n";
        cout << "2. Benchmark\n";
        cout << "3. Tree of int\n";
        cout << "4. Tree of double\n";
        cout << "5. Tree of complex\n";
        cout << "6. Tree of string\n";
        cout << "7. Tree of students\n";
        cout << "8. Tree of teachers\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        if (!Read(choice)) {
            choice = -1;
            continue;
        }

        if (choice == 1) {
            RunAllTests();
        }

        else if (choice == 2) {
            int maxCount;
            cout << "Max count (for example 1000000 or 10000000): ";
            if (Read(maxCount)) {
                RunBenchmark(maxCount);
            }
        }

        else if (choice == 3) {
            ShowTreeMenu<int>("int");
        }

        else if (choice == 4) {
            ShowTreeMenu<double>("double");
        }

        else if (choice == 5) {
            ShowTreeMenu<Complex>("complex");
        }

        else if (choice == 6) {
            ShowTreeMenu<string>("string");
        }

        else if (choice == 7) {
            ShowTreeMenu<Student>("students");
        }

        else if (choice == 8) {
            ShowTreeMenu<Teacher>("teachers");
        }
    }
}
