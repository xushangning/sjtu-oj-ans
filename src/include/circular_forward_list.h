#ifndef CIRCULAR_FORWARD_LIST_H_
#define CIRCULAR_FORWARD_LIST_H_

namespace sx
{

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
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void erase(iterator i);
    void print() const noexcept;
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
void circular_forward_list<T>::push_back(const T& value)
{
    if (empty()) {
        head = new node {value, nullptr};
        head->next = tail = head;
    }
    else
        tail->next = head = new node {value, head};
    ++var_size;
}

template <typename T>
void circular_forward_list<T>::push_back(T&& value)
{
    if (empty()) {
        head = new node {value, nullptr};
        head->next = tail = head;
    }
    else
        tail->next = head = new node {value, head};
    ++var_size;
}

template <typename T>
void circular_forward_list<T>::pop_back()
{
    if (!empty()) {
        if (head->next) {
            node * temp = head;
            head = head->next;
            delete temp;
        }
        else {
            delete head;
            head = nullptr;  // must be set to the null pointer
        }
        --var_size;
    }
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
void circular_forward_list<T>::print() const noexcept
{
    node * p = head;
    while (p != tail) {
        std::cout << p->data << ' ';
        p = p->next;
    }
    std::cout << p->data << std::endl;
}

}

#endif // CIRCULAR_FORWARD_LIST_H_
