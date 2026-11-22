#pragma once

#undef NDEBUG
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include "BinaryTree.h"
#include "Complex.h"
#include "Person.h"

inline int DoubleInt(int x) { // умножить на 2
    return x * 2;
}

inline bool IsEven(int x) { // чётное ли число
    return x % 2 == 0;
}

inline int Sum(int a, int b) { // сумма для reduce
    return a + b;
}

inline int SpecFunction(int a, int b) { // 2a + 3b
    return 2 * a + 3 * b;
}

inline Complex MultiplyByI(Complex c) { // умножить комплексное число на i
    return c * Complex(0, 1);
}

inline bool IsYoung(Student s) { // родился в 2005 или позже
    return s.GetBirthYear() >= 2005;
}

inline BinaryTree<int> MakeSampleTree() { // дерево основное
    BinaryTree<int> tree;
    tree.Insert(20);
    tree.Insert(10);
    tree.Insert(30);
    tree.Insert(5);
    tree.Insert(15);
    return tree;
}

inline std::string InOrderString(const BinaryTree<int>& tree) { // все элементы по возрастанию одной строкой
    std::string text = "";
    TreeEnumerator<int> it = tree.GetEnumerator();
    while (it.MoveNext()) {
        if (!text.empty()) {
            text += " ";
        }
        text += std::to_string(it.GetCurrent());
    }
    return text;
}

inline void TestEmpty() { // пустое дерево
    BinaryTree<int> tree;
    assert(tree.GetCount() == 0);
    assert(tree.IsEmpty());
    assert(tree.GetHeight() == 0);
    assert(!tree.Contains(5));
    assert(tree.ToString() == "");
    std::cout << "[OK] Empty" << std::endl;
}

inline void TestInsert() { // вставка и повторы
    BinaryTree<int> tree = MakeSampleTree();
    assert(tree.GetCount() == 5);
    assert(tree.Contains(20) and tree.Contains(10) and tree.Contains(30) and
                 tree.Contains(5) and tree.Contains(15));
    assert(!tree.Contains(7));
    assert(!tree.Insert(10));
    assert(tree.GetCount() == 5);
    std::cout << "[OK] Insert" << std::endl;
}

inline void TestRemove() { // удаление
    BinaryTree<int> tree = MakeSampleTree();
    tree.Remove(5);
    assert(!tree.Contains(5) and tree.GetCount() == 4);
    tree.Remove(10);
    assert(!tree.Contains(10) and tree.Contains(15) and tree.GetCount() == 3);

    BinaryTree<int> other = MakeSampleTree();
    other.Remove(20);
    assert(!other.Contains(20) and other.GetCount() == 4);
    assert(InOrderString(other) == "5 10 15 30");
    std::cout << "[OK] Remove" << std::endl;
}

inline void TestFind() { // поиск элемента
    BinaryTree<int> tree = MakeSampleTree();
    assert(tree.Find(15) == 15);

    bool thrown = false; //проверка на ошибку
    try {
        tree.Find(99);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);
    std::cout << "[OK] Find" << std::endl;
}

inline void TestBalance() { // AVL игры с высотой
    BinaryTree<int> tree;
    for (int i = 1; i <= 1000; i++) {
        tree.Insert(i);
    }
    assert(tree.GetCount() == 1000);
    assert(tree.GetHeight() <= 14);

    for (int i = 1; i <= 1000; i += 2) {
        tree.Remove(i);
    }
    assert(tree.GetCount() == 500);
    assert(tree.GetHeight() <= 13);

    bool sorted = true;
    for (int i = 1; i < tree.GetCount(); i++) {
        if (!(tree.Get(i - 1) < tree.Get(i))) {
            sorted = false;
        }
    }
    assert(sorted);
    std::cout << "[OK] Balance" << std::endl;
}

inline void TestCopy() { // копирование,
    BinaryTree<int> tree = MakeSampleTree();
    BinaryTree<int> copy(tree);
    copy.Insert(99);
    assert(copy.Contains(99) and !tree.Contains(99));

    BinaryTree<int> assigned; //присваивание
    assigned.Insert(1);
    assigned = tree;
    assigned.Remove(20);
    assert(!assigned.Contains(1) and tree.Contains(20));

    BinaryTree<int>& same = tree; //одно и то же
    tree = same;
    assert(tree.GetCount() == 5);

    tree.Clear(); //удаление
    assert(tree.IsEmpty() and tree.GetHeight() == 0);
    tree.Insert(7);
    assert(tree.Contains(7) and tree.GetCount() == 1);
    std::cout << "[OK] Copy" << std::endl;
}

inline void TestGet() { // элемент по номеру и неверный номер
    BinaryTree<int> tree = MakeSampleTree();
    assert(tree.Get(0) == 5 and tree.Get(2) == 15 and tree.Get(4) == 30);

    bool thrownLow = false; //на ошибку
    try {
        tree.Get(-1);
    } catch (const std::out_of_range&) {
        thrownLow = true;
    }
    bool thrownHigh = false;
    try {
        tree.Get(5);
    } catch (const std::out_of_range&) {
        thrownHigh = true;
    }
    assert(thrownLow and thrownHigh);
    std::cout << "[OK] Get" << std::endl;
}

inline void TestEnumerator() { // итератор
    BinaryTree<int> tree = MakeSampleTree();
    TreeEnumerator<int> it = tree.GetEnumerator();
    std::string text = "";
    while (it.MoveNext()) {
        text += std::to_string(it.GetCurrent()) + " ";
    }
    assert(text == "5 10 15 20 30 ");
    assert(!it.MoveNext());

    it.Reset();
    assert(it.MoveNext() and it.GetCurrent() == 5);

    TreeEnumerator<int> fresh = tree.GetEnumerator();
    bool thrown = false;
    try {
        fresh.GetCurrent();
    } catch (const std::out_of_range&) {
        thrown = true;
    }
    assert(thrown);

    BinaryTree<int> empty; //пустой
    TreeEnumerator<int> emptyIt = empty.GetEnumerator();
    assert(!emptyIt.MoveNext());
    std::cout << "[OK] Enumerator" << std::endl;
}

inline void TestMapWhereReduce() { // map, where, reduce
    BinaryTree<int> tree = MakeSampleTree();

    BinaryTree<int> doubled = tree.Map(DoubleInt);
    assert(doubled.Contains(10) and doubled.Contains(60) and doubled.GetCount() == 5);
    assert(tree.Contains(5) and !tree.Contains(60));

    BinaryTree<int> even = tree.Where(IsEven);
    assert(even.GetCount() == 3 and even.Contains(10) and !even.Contains(5));

    assert(tree.Reduce(Sum, 0) == 80);

    BinaryTree<int> spec;
    spec.Insert(1);
    spec.Insert(2);
    spec.Insert(3);
    assert(spec.Reduce(SpecFunction, 4) == 144);
    std::cout << "[OK] Map/Where/Reduce" << std::endl;
}

inline void TestToFromString() { // сохранение в строку и чтение из строки
    BinaryTree<int> tree = MakeSampleTree();
    assert(tree.ToString() == "20 10 5 15 30");

    BinaryTree<int> restored;
    restored.FromString(tree.ToString());
    assert(restored == tree);

    restored.FromString("");
    assert(restored.IsEmpty());

    BinaryTree<int> safe = MakeSampleTree();
    bool thrown = false;
    try {
        safe.FromString("1 2 abc 3");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);
    assert(safe == MakeSampleTree());
    std::cout << "[OK] ToString/FromString" << std::endl;
}

inline void TestFormatString() { // сохранение по шаблону {К}(Л)[П]
    BinaryTree<int> tree = MakeSampleTree();
    assert(tree.ToFormatString("{К}(Л)[П]") == "{20}({10}({5})[{15}])[{30}]");
    assert(tree.ToFormatString("(L){K}[P]") == "(({5}){10}[{15}]){20}[{30}]");

    bool thrownLength = false;
    try {
        tree.ToFormatString("{K}(L)");
    } catch (const std::invalid_argument&) {
        thrownLength = true;
    }
    assert(thrownLength);

    bool thrownLetters = false;
    try {
        tree.ToFormatString("{K}(K)[P]");
    } catch (const std::invalid_argument&) {
        thrownLetters = true;
    }
    assert(thrownLetters);

    BinaryTree<int> empty;
    assert(empty.ToFormatString("{K}(L)[P]") == "");
    std::cout << "[OK] Format string" << std::endl;
}

inline void TestSubtree() { // извлечение и поиск поддерева
    BinaryTree<int> tree = MakeSampleTree();
    BinaryTree<int> sub = tree.ExtractSubtree(10);
    assert(sub.GetCount() == 3 and sub.ToString() == "10 5 15");
    assert(tree.GetCount() == 5);
    assert(tree.ContainsSubtree(sub));

    sub.Insert(99);
    assert(!tree.ContainsSubtree(sub));

    BinaryTree<int> empty;
    assert(tree.ContainsSubtree(empty));

    bool thrown = false;
    try {
        tree.ExtractSubtree(99);
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);
    std::cout << "[OK] Subtree" << std::endl;
}

inline void TestOperators() { // операторы +, ==, !=
    BinaryTree<int> a;
    a.Insert(10);
    a.Insert(20);
    BinaryTree<int> b;
    b.Insert(20);
    b.Insert(30);
    b.Insert(40);

    BinaryTree<int> c = a + b;
    assert(c.GetCount() == 4 and c.Contains(10) and c.Contains(40));
    assert(a.GetCount() == 2 and b.GetCount() == 3);

    BinaryTree<int> x;
    x.Insert(1);
    x.Insert(2);
    x.Insert(3);
    BinaryTree<int> y;
    y.Insert(3);
    y.Insert(2);
    y.Insert(1);
    assert(x == y);
    assert(!(x != y));
    assert(a != b);
    assert(!(a == c));
    std::cout << "[OK] Operators" << std::endl;
}

inline void TestDouble() { // дерево вещественных чисел
    BinaryTree<double> tree;
    tree.Insert(2.5);
    tree.Insert(-1.25);
    tree.Insert(3.75);
    assert(tree.GetCount() == 3 and tree.Get(0) == -1.25);

    BinaryTree<double> restored;
    restored.FromString(tree.ToString());
    assert(restored == tree);
    std::cout << "[OK] Double" << std::endl;
}

inline void TestComplex() { // комплексные числа и дерево из них
    Complex product = Complex(1, 2) * Complex(3, 4);
    assert(product == Complex(-5, 10));
    assert((Complex(1, 2) + Complex(3, 4)) == Complex(4, 6));
    assert(Complex(3, 4).Abs() == 5);
    assert(Complex(1, 1) < Complex(3, 4));
    assert(Complex(3, 4) < Complex(4, 3));

    BinaryTree<Complex> tree;
    tree.Insert(Complex(3, 4));
    tree.Insert(Complex(4, 3));
    tree.Insert(Complex(1, 2));
    assert(tree.GetCount() == 3);

    BinaryTree<Complex> restored;
    restored.FromString(tree.ToString());
    assert(restored == tree);

    BinaryTree<Complex> rotated = tree.Map(MultiplyByI);
    assert(rotated.Contains(Complex(-4, 3)));

    bool thrown = false;
    try {
        restored.FromString("(1,2) (3;4)");
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    bool thrownCut = false;
    try {
        restored.FromString("(1,2) (3");
    } catch (const std::invalid_argument&) {
        thrownCut = true;
    }
    assert(thrownCut);
    std::cout << "[OK] Complex" << std::endl;
}

inline void TestStringType() { // дерево строк
    BinaryTree<std::string> tree;
    tree.FromString("pear apple fig");
    assert(tree.GetCount() == 3);
    assert(tree.Get(0) == "apple" and tree.Get(2) == "pear");

    BinaryTree<std::string> restored;
    restored.FromString(tree.ToString());
    assert(restored == tree);
    std::cout << "[OK] String" << std::endl;
}

inline void TestStudents() { // дерево студентов
    BinaryTree<Student> tree;
    tree.Insert(Student(4510, 123456, "Ivanov", "Ivan", "Ivanovich", 2005, "B25-507"));
    tree.Insert(Student(4510, 100000, "Petrov", "Petr", "Petrovich", 2004, "B25-517"));
    tree.Insert(Student(4511, 555555, "Sidorova", "Anna", "Sergeevna", 2006, "B25-527"));
    assert(tree.GetCount() == 3);

    bool added = tree.Insert(Student(4510, 123456, "Other", "Name", "Here", 2000, "X"));
    assert(!added and tree.GetCount() == 3);

    Student key;
    key.SetPerson(4510, 123456, "", "", "", 0);
    assert(tree.Find(key).GetFullName() == "Ivanov Ivan Ivanovich");
    assert(tree.Get(0).GetLastName() == "Petrov");

    BinaryTree<Student> restored;
    restored.FromString(tree.ToString());
    assert(restored == tree and restored.Find(key).GetGroup() == "B25-507");

    assert(tree.Where(IsYoung).GetCount() == 2);

    bool thrownCut = false;
    try {
        restored.FromString("4510 1 Ivanov Ivan");
    } catch (const std::invalid_argument&) {
        thrownCut = true;
    }
    assert(thrownCut);

    bool thrownNegative = false;
    try {
        Student bad(-1, 1, "A", "B", "C", 2000, "G");
    } catch (const std::invalid_argument&) {
        thrownNegative = true;
    }
    assert(thrownNegative);
    std::cout << "[OK] Student" << std::endl;
}

inline void TestTeachers() { // дерево преподавателей
    BinaryTree<Teacher> tree;
    tree.FromString("4000 111111 Smirnov Oleg Ivanovich 1970 Kafedra22 "
                    "4000 222222 Orlova Irina Petrovna 1980 Kafedra42");
    assert(tree.GetCount() == 2);
    assert(tree.Get(0).GetDepartment() == "Kafedra22");

    BinaryTree<Teacher> restored;
    restored.FromString(tree.ToString());
    assert(restored == tree);
    std::cout << "[OK] Teacher" << std::endl;
}

inline void RunAllTests() { // запустить все тесты
    TestEmpty();
    TestInsert();
    TestRemove();
    TestFind();
    TestBalance();
    TestCopy();
    TestGet();
    TestEnumerator();
    TestMapWhereReduce();
    TestToFromString();
    TestFormatString();
    TestSubtree();
    TestOperators();
    TestDouble();
    TestComplex();
    TestStringType();
    TestStudents();
    TestTeachers();
    std::cout << "All tests passed" << std::endl;
}
