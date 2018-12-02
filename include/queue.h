#ifndef QUEUE_H_
#define QUEUE_H_

#include "forward_list.h"

namespace sx
{

template <typename T, typename Container = forward_list<T>>
class queue
{
protected:
    Container c;
public:
    T& front() { return c.front(); }
    const T& front() const { return c.front(); }
    T& back() { return c.back(); }
    const T& back() const { return c.back(); }
    bool empty() const { return c.empty(); }
    int size() const { return c.size(); }
    void push(const T& value) { c.push_back(value); }
    void push(T&& value) { c.push_back(value); }
    void pop() { c.pop_front(); }
};

}

#endif // QUEUE_H_
