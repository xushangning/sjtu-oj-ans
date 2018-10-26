#ifndef FORWARD_LIST_H_
#define FORWARD_LIST_H_

namespace sx
{

template <typename T>
class rforward_list
{
private:
    struct node
    {
        T data;
        node * next;
    };
    int var_size;
    node * tail;
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
    };

    rforward_list() noexcept : var_size(0), tail(nullptr) {}
    ~rforward_list();

    int size() const noexcept { return var_size; }
    bool empty() const noexcept { return !var_size; }
    T& back() noexcept { return tail->data; }
    const T& back() const noexcept { return tail->data; }
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
};

template <typename T>
rforward_list<T>::~rforward_list()
{
    node * temp;
    while (tail)
    {
        temp = tail;
        tail = tail->next;
        delete temp;
    }
}

template <typename T>
void rforward_list<T>::push_back(const T& value)
{
    if (empty())
        tail = new node {value, nullptr};
    else
        tail = new node {value, tail};
    ++var_size;
}

template <typename T>
void rforward_list<T>::push_back(T&& value)
{
    if (empty())
        tail = new node {value, nullptr};
    else
        tail = new node {value, tail};
    ++var_size;
}

template <typename T>
void rforward_list<T>::pop_back()
{
    if (!empty()) {
        if (tail->next) {
            node * temp = tail;
            tail = tail->next;
            delete temp;
        }
        else {
            delete tail;
            tail = nullptr;  // must be set to the null pointer
        }
        --var_size;
    }
}

}

#endif // FORWARD_LIST_H_
