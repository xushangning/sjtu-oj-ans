#ifndef STACK_H_
#define STACK_H_

#include "rforward_list.h"

namespace sx
{

template <typename T, typename Container = rforward_list<T>>
class stack
{
public:
    const T& top() const { return c.back(); }
    T& top() { return c.back(); }
    bool empty() const { return c.empty(); }
    int size() const { return c.size(); }
    void push(const T& value) { return c.push_back(value); }
    void push(T&& value) { return c.push_back(value); }
    void pop() { return c.pop_back(); }
protected:
    Container c;
};

}

#endif // STACK_H_
