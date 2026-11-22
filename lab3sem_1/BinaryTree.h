#pragma once

#include <stdexcept>
#include "Node.h"
#include <string>
#include <sstream>
#include <iostream>

template <typename T>
class TreeEnumerator {
private:
    Node<T>* root;
    Node<T>* current;
    bool started;

    Node<T>* FindNext(const T& value) const {  //показать следующий по величине
        Node<T>* candidate = nullptr;
        Node<T>* node = this->root;
        while (node != nullptr) {
            if (value < node->value) {
                candidate = node;
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return candidate;
    }

public:
    explicit TreeEnumerator(Node<T>* root) { //конструктор
        this->root = root;
        this->current = nullptr;
        this->started = false;
    }

    bool MoveNext() { //двигается вперед
        if (!this->started) {
            this->started = true;
            this->current = this->root;
            if (this->current == nullptr) {
                return false;
            }
            while (this->current->left != nullptr) {
                this->current = this->current->left;
            }
            return true;
        }
        if (this->current == nullptr) {
            return false;
        }
        this->current = FindNext(this->current->value);
        return this->current != nullptr;
    }

    const T& GetCurrent() const { //получить текущий
        if (this->current == nullptr) {
            throw std::out_of_range("TreeEnumerator::GetCurrent: no current element");
        }
        return this->current->value;
    }

    void Reset() { // обнулить
        this->current = nullptr;
        this->started = false;
    }
};

template <typename T>
class BinaryTree {
private:
    Node<T>* root;
    int count;

    int Height(Node<T>* node) const { // узнать высоту
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    void UpdateHeight(Node<T>* node) { // обновить высоту
        int leftHeight = Height(node->left);
        int rightHeight = Height(node->right);
        if (leftHeight > rightHeight) {
            node->height = leftHeight + 1;
        } else {
            node->height = rightHeight + 1;
        }
    }

    int BalanceFactor(Node<T>* node) { // проверка перевеса(если больше 1 то перевес есть влево)
        return Height(node->left) - Height(node->right);
    }

    Node<T>* RotateRight(Node<T>* node) { // поворот вправо, левый член теперь верхний
        Node<T>* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        UpdateHeight(node);
        UpdateHeight(newRoot);
        return newRoot;
    }

    Node<T>* RotateLeft(Node<T>* node) { // поворот влево, правый член верхний
        Node<T>* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        UpdateHeight(node);
        UpdateHeight(newRoot);
        return newRoot;
    }

    Node<T>* Rebalance(Node<T>* node) { // выровнять
        UpdateHeight(node);
        int balance = BalanceFactor(node);
        if (balance > 1) {
            if (BalanceFactor(node->left) < 0) {
                node->left = RotateLeft(node->left);
            }
            return RotateRight(node);
        }
        if (balance < -1) {
            if (BalanceFactor(node->right) > 0) {
                node->right = RotateRight(node->right);
            }
            return RotateLeft(node);
        }
        return node;
    }

    Node<T>* InsertNode(Node<T>* node, const T& value) { // вставка
        if (node == nullptr) {
            return new Node<T>(value);
        }
        if (value < node->value) {
            node->left = InsertNode(node->left, value);
        } else if (node->value < value) {
            node->right = InsertNode(node->right, value);
        } else {
            return node;
        }
        return Rebalance(node);
    }

    Node<T>* RemoveNode(Node<T>* node, const T& value) { //удаление элемента
        if (node == nullptr) {
            throw std::invalid_argument("BinaryTree::Remove: element not found");
        }

        if (value < node->value) {
            node->left = RemoveNode(node->left, value);
        } else if (node->value < value) {
            node->right = RemoveNode(node->right, value);
        } else {
            if (node->left == nullptr or node->right == nullptr) {
                Node<T>* child = node->left;
                if (child == nullptr) {
                    child = node->right;
                }
                delete node;
                return child;
            }
            Node<T>* minNode = node->right;
            while (minNode->left != nullptr) {
                minNode = minNode->left;
            }
            T minValue = minNode->value;
            node->value = minValue;
            node->right = RemoveNode(node->right, minValue);
        }
        return Rebalance(node);
    }

    Node<T>* FindNode(Node<T>* node, const T& value) const { //найти ноду
        while (node != nullptr) {
            if (value < node->value) {
                node = node->left;
            } else if (node->value < value) {
                node = node->right;
            } else {
                return node;
            }
        }
        return nullptr;
    }

    Node<T>* CopyNode(Node<T>* node) { // копируем все
        if (node == nullptr) {
            return nullptr;
        }
        Node<T>* copy = new Node<T>(node->value);
        copy->height = node->height;
        copy->left = CopyNode(node->left);
        copy->right = CopyNode(node->right);
        return copy;
    }

    void DeleteNode(Node<T>* node) { //удалить полностью
        if (node == nullptr) {
            return;
        }
        DeleteNode(node->left);
        DeleteNode(node->right);
        delete node;
    }

public:
    BinaryTree() { //конструктор по умолч
        this->root = nullptr;
        this->count = 0;
    }

    BinaryTree(const BinaryTree& other) { // конструктор копирования
        this->root = CopyNode(other.root);
        this->count = other.count;
    }

    BinaryTree& operator=(const BinaryTree& other) { //приравниваем
        if (this != &other) {
            DeleteNode(this->root);
            this->root = CopyNode(other.root);
            this->count = other.count;
        }
        return *this;
    }

    ~BinaryTree() { //деструктор
        DeleteNode(this->root);
    }

    bool Insert(const T& value) { //вставить
        if (Contains(value)) {
            return false;
        }
        this->root = InsertNode(this->root, value);
        this->count++;
        return true;
    }

    void Remove(const T& value) { //удалить
        this->root = RemoveNode(this->root, value);
        this->count--;
    }

    bool Contains(const T& value) const { // есть ли нода
        return FindNode(this->root, value) != nullptr;
    }

    const T& Find(const T& value) const { //есть ли элемент(и вернуть его)
        Node<T>* node = FindNode(this->root, value);
        if (node == nullptr) {
            throw std::invalid_argument("BinaryTree::Find: element not found");
        }
        return node->value;
    }

    int GetCount() const { //получить
        return this->count;
    }

    bool IsEmpty() const { //проверить пуст ли
        return this->count == 0;
    }

    int GetHeight() const { //получить высоту
        return Height(this->root);
    }

    void Clear() { //полностью очистить
        DeleteNode(this->root);
        this->root = nullptr;
        this->count = 0;
    }

    T Get(int index) const { //получить элемент в зависимости от функции
        if (index < 0 or index >= this->count) {
            throw std::out_of_range("BinaryTree::Get: index out of range");
        }
        T result;
        int position = 0;
        GetNode(this->root, index, position, result);
        return result;
    }

    TreeEnumerator<T> GetEnumerator() const { //создает новый итератор для дерева
        return TreeEnumerator<T>(this->root);
    }

    BinaryTree<T> Map(T (*func)(T)) const { // применение функции к дереву
        BinaryTree<T> result;
        MapNode(this->root, func, result);
        return result;
    }

    BinaryTree<T> Where(bool (*predicate)(T)) const { //по условию
        BinaryTree<T> result;
        WhereNode(this->root, predicate, result);
        return result;
    }

    T Reduce(T (*func)(T, T), const T& start) const { //все сворачивает в одно
        T result = start;
        ReduceNode(this->root, func, result);
        return result;
    }

    std::string ToString() const { // записать через пробел(в конце пробел убираем)
        std::ostringstream out;
        ToStringNode(this->root, out);
        std::string text = out.str();
        if (!text.empty()) {
            text.pop_back();
        }
        return text;
    }

    void FromString(const std::string& text) { // из строки в дерево
        BinaryTree<T> result;
        std::istringstream in(text);
        T value;
        while (true) {
            in >> std::ws;
            if (in.eof()) {
                break;
            }
            if (!(in >> value)) {
                throw std::invalid_argument("BinaryTree::FromString: invalid format");
            }
            result.Insert(value);
        }
        *this = result;
    }

    std::string ToFormatString(const std::string& format) const { // превращаем в буквы для форматирования
        std::string pattern = format;
        ReplaceAll(pattern, "К", "K");
        ReplaceAll(pattern, "Л", "L");
        ReplaceAll(pattern, "П", "P");
        if (pattern.size() != 9) {
            throw std::invalid_argument("BinaryTree::ToFormatString: invalid format");
        }
        std::string parts = {pattern[1], pattern[4], pattern[7]};
        if (parts.find('K') == std::string::npos or
            parts.find('L') == std::string::npos or
            parts.find('P') == std::string::npos) {
            throw std::invalid_argument("BinaryTree::ToFormatString: invalid format");
        }
        std::ostringstream out;
        if (this->root != nullptr) {
            FormatNode(this->root, pattern, out);
        }
        return out.str();
    }

    BinaryTree<T> ExtractSubtree(const T& value) { //вырезать из дерева поддерево
        Node<T>* node = FindNode(this->root, value);
        if (node == nullptr) {
            throw std::invalid_argument("BinaryTree::ExtractSubtree: element not found");
        }
        BinaryTree<T> result;
        result.root = CopyNode(node);
        result.count = CountNodes(result.root);
        return result;
    }

    bool ContainsSubtree(const BinaryTree<T>& other) const { //есть ли поддерево в дереве
        if (other.root == nullptr) {
            return true;//потому что всегда пустое дерево поддерево
        }
        Node<T>* node = FindNode(this->root, other.root->value);
        return IsSameTree(node, other.root); //одинаковы ли деревья
    }

    BinaryTree<T> operator+(const BinaryTree<T>& other) const { //просто к первому добавляем все вторые
        BinaryTree<T> result(*this);
        result.InsertAll(other.root);
        return result;
    }

    bool operator==(const BinaryTree<T>& other) const { //сравниваем
        if (this->count != other.count) {
            return false;
        }
        return ContainsAll(this->root, other);//проверяем есть ли каждый элемент в текущем
    }

    bool operator!=(const BinaryTree<T>& other) const { //не равно)
        return !(*this == other);
    }

private:
    void GetNode(Node<T>* node, int index, int& position, T& result) const {
        if (node == nullptr) {
            return; //ноды нет, выходим
        }
        GetNode(node->left, index, position, result); //ходим влево максимально
        if (position == index) {
            result = node->value;
        }
        position++;
        GetNode(node->right, index, position, result);
    }

    void MapNode(Node<T>* node, T (*func)(T), BinaryTree<T>& result) const { // делаем map со всеми членами
        if (node == nullptr) {
            return;
        }
        MapNode(node->left, func, result);
        result.Insert(func(node->value));
        MapNode(node->right, func, result);
    }

    void WhereNode(Node<T>* node, bool (*predicate)(T), BinaryTree<T>& result) const { //аналогично как с мар
        if (node == nullptr) {
            return;
        }
        WhereNode(node->left, predicate, result);
        if (predicate(node->value)) {
            result.Insert(node->value);
        }
        WhereNode(node->right, predicate, result);
    }

    void ReduceNode(Node<T>* node, T (*func)(T, T), T& result) const { // обьединяем в одно в результ
        if (node == nullptr) {
            return;
        }
        ReduceNode(node->left, func, result);
        result = func(node->value, result);
        ReduceNode(node->right, func, result);
    }

    void InsertAll(Node<T>* node) { // в текущее дерево все элементы
        if (node == nullptr) {
            return;
        }
        Insert(node->value);
        InsertAll(node->left);
        InsertAll(node->right);
    }

    bool ContainsAll(Node<T>* node, const BinaryTree<T>& other) const { // есть ли каждое значение дерева в другом
        if (node == nullptr) {
            return true;
        }
        if (!other.Contains(node->value)) {
            return false;
        }
        return ContainsAll(node->left, other) and ContainsAll(node->right, other); //ходим влево и вправо
    }

    void ToStringNode(Node<T>* node, std::ostringstream& out) const { // запсиать все через пробел
        if (node == nullptr) {
            return;
        }
        out << node->value << " ";
        ToStringNode(node->left, out);
        ToStringNode(node->right, out);
    }

    void FormatNode(Node<T>* node, const std::string& pattern, std::ostringstream& out) const {//раставляем скобки
        for (int i = 0; i < 9; i += 3) {
            char open = pattern[i];
            char part = pattern[i + 1];
            char close = pattern[i + 2];
            if (part == 'K') {
                out << open << node->value << close;
            } else if (part == 'L' and node->left != nullptr) {
                out << open;
                FormatNode(node->left, pattern, out);
                out << close;
            } else if (part == 'P' and node->right != nullptr) {
                out << open;
                FormatNode(node->right, pattern, out);
                out << close;
            }
        }
    }

    void ReplaceAll(std::string& text, const std::string& from, const std::string& to) const {//что меняем где и на что
        size_t position = text.find(from);//ищем первое вхождение
        while (position != std::string::npos) {
            text.replace(position, from.size(), to);//замена откуда сколько и на что
            position = text.find(from, position + to.size());// ищем, но с восстановленного куска
        }
    }

    int CountNodes(Node<T>* node) const { // мощность дерева
        if (node == nullptr) {
            return 0;
        }
        return 1 + CountNodes(node->left) + CountNodes(node->right);
    }

    bool IsSameTree(Node<T>* first, Node<T>* second) const { //одинаковы ли деревья
        if (first == nullptr and second == nullptr) {
            return true;
        }
        if (first == nullptr or second == nullptr) {
            return false;
        }
        if (first->value < second->value or second->value < first->value) {
            return false;
        }
        return IsSameTree(first->left, second->left) and IsSameTree(first->right, second->right);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinaryTree<T>& tree) {
    out << tree.ToString();
    return out;
}
