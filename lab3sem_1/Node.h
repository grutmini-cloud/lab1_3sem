#pragma once

template <typename T>
struct Node {
    T value;  //значение узла
    Node* left; // указатель влево
    Node* right; // указатель вправо
    int height; // номер узла(типо как глубоко он) высота

    explicit Node(const T& value) {
        this->value = value;
        this->left = nullptr;
        this->right = nullptr;
        this->height = 1;
    }
};