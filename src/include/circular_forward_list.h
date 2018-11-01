#ifndef CIRCULAR_FORWARD_LIST_H_
#define CIRCULAR_FORWARD_LIST_H_

#include <iostream>

namespace sx
{

template <typename T> class circular_forward_list;

template <typename T>
std::ostream& operator<<(std::ostream& os, const circular_forward_list<T>& l);

template <typename T>
class circular_forward_list
{
private:
    struct node
    {
        T data;
        node * next;
    };
    int var_size;
    node * head, * tail;
public:
    class const_iterator
    {
    private:
        const node * p;
    public:
        const_iterator() = default;
        const_iterator(const node * p_) noexcept : p(p_) {}

        const_iterator operator++() noexcept { return p = p->next; }
        const_iterator operator++(int) noexcept
        {
            node * temp = p;
            p = p->next;
            return temp;
        }
        const T & operator*() const noexcept { return p->data; }

        friend class circular_forward_list;
    };

    class iterator
    {
    private:
        node * p;
    public:
        iterator() = default;
        iterator(node * p_) noexcept : p(p_) {}

        iterator operator++() noexcept { return p = p->next; }
        iterator operator++(int) noexcept
        {
            node * temp = p;
            p = p->next;
            return temp;
        }
        T& operator*() const noexcept { return p->data; }

        friend class circular_forward_list;
    };

    circular_forward_list() noexcept : var_size(0), head(nullptr), tail(nullptr) {}
    ~circular_forward_list();

    int size() const noexcept { return var_size; }
    bool empty() const noexcept { return !var_size; }
    iterator begin() noexcept { return head; }
    T& back() noexcept { return head->data; }
    const T& back() const noexcept { return head->data; }
    void erase(iterator i);

    friend std::ostream& operator<< <>(std::ostream& os, const circular_forward_list& l);
};

// watch out for n = 1
template <typename T>
circular_forward_list<T>::~circular_forward_list()
{
    node * temp;
    while (head != tail)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
    delete head;
}

template <typename T>
void circular_forward_list<T>::erase(iterator i)
{
    node * p = head;
    while (p->next != i.p)  // find the previous node
        p = p->next;
    p->next = i.p->next;
    if (tail == i.p)
        tail = p;
    delete i.p;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const circular_forward_list<T>& l)
{
    if (l.head) {
        typename circular_forward_list<T>::node * p = l.head->next;
        os << l.head->data;
        while (p != l.head) {
            os << ' ' << p->data;
            p = p->next;
        }
    }
    return os;
}

}

#endif // CIRCULAR_FORWARD_LIST_H_
