#pragma once

#include <cmath>
#include <iostream>

class Complex {
private:
    double re; //реальная часть
    double im; //мнимая часть

public:
    Complex() { // конструктор по умолч
        this->re = 0;
        this->im = 0;
    }

    Complex(double re, double im) { // конструктор
        this->re = re;
        this->im = im;
    }

    double GetRe() const { //получить дейст часть
        return this->re;
    }

    double GetIm() const { //получить мнимую часть
        return this->im;
    }

    double Abs() const { // найти значение по моделю
        return std::sqrt(this->re * this->re + this->im * this->im);
    }

    Complex operator+(const Complex& other) const { // сложение комплексных чисел
        return Complex(this->re + other.re, this->im + other.im);
    }

    Complex operator-(const Complex& other) const { // вычитание комплексных чисел
        return Complex(this->re - other.re, this->im - other.im);
    }

    Complex operator*(const Complex& other) const { // умножение по формуле
        return Complex(this->re * other.re - this->im * other.im,
                       this->re * other.im + this->im * other.re);
    }

    bool operator==(const Complex& other) const { // сравнение
        return this->re == other.re and this->im == other.im;
    }

    bool operator<(const Complex& other) const { //сравнение
        double thisAbs = this->Abs();
        double otherAbs = other.Abs();
        if (thisAbs != otherAbs) {
            return thisAbs < otherAbs;
        }
        if (this->re != other.re) {
            return this->re < other.re;
        }
        return this->im < other.im;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Complex& c) { // как печатать ре+мн
    out << "(" << c.GetRe() << "," << c.GetIm() << ")";
    return out;
}

inline std::istream& operator>>(std::istream& in, Complex& c) { // обратно
    char open = 0;
    char comma = 0;
    char close = 0;
    double re = 0;
    double im = 0;
    in >> open >> re >> comma >> im >> close;
    if (!in or open != '(' or comma != ',' or close != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    c = Complex(re, im);
    return in;
}
