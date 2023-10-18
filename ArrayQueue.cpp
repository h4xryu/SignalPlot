#include "ArrayQueue.h"

template<typename T>
ArrayQueue<T>::~ArrayQueue<T>(){
    delete Q;
}

template<typename T>
void ArrayQueue<T>::push(const T& data){
    try {
        if (size() == N) throw QueueFull();

        Q[r] = data;
        r = (r+1) % N;
        ++n;
    }
    catch (QueueFull e){
        std::cout << "큐 꽉참." << std::endl;
        exit(2);
    }
}

template<typename T>
T& ArrayQueue<T>::pop(){
    try {
        if (empty()) throw QueueEmpty();

        f = (f+1) % N;
        --n;
    } catch (QueueEmpty e){
        std::cout << "큐 비었음." << std::endl;
        exit(2);
    }
}

template<typename T>
bool ArrayQueue<T>::empty() const
{
    return (n == 0);
}

template<typename T>
int ArrayQueue<T>::size() const
{
    return n;
}

template<typename T>
void ArrayQueue<T>::print() const
{
    std::cout << "front [ ";
    for (int i = f; i != r; i = (i + 1) % N)
    {
        std::cout << Q[i] << " ";
    }
    std::cout << "] rear\n";
}
