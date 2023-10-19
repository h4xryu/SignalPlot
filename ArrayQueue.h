#ifndef ARRAYQUEUE_H
#define ARRAYQUEUE_H

class QueueEmpty{};
class QueueFull{};
#include<iostream>

template<typename T>
class ArrayQueue{

public:
    ArrayQueue(int Capacity = 100) : N(Capacity), n(0), f(0), r(0), Q(new T[Capacity]) {}
        ~ArrayQueue();

        void push(const T& data);
        T& pop();
        T& front() const;
        bool empty() const;
        int size() const;
        void print() const;

private:
        int N;
        int n;
        int f, r;
        T* Q;

};


#endif // ARRAYQUEUE_H
