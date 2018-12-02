#ifndef FORWARD_LIST_H_
#define FORWARD_LIST_H_

namespace sx
{

template <typename T>
class forward_list
{
private:
    struct node
    {
        T key;
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
        const T & operator*() const noexcept { return p->key; }
    };

    forward_list() noexcept : var_size(0), head(nullptr), tail(nullptr) {}
    ~forward_list();

    int size() const noexcept { return var_size; }
    bool empty() const noexcept { return !var_size; }
    T& front() noexcept { return head->key; }
    const T& front() const noexcept { return head->key; }
    T& back() noexcept { return tail->key; }
    const T& back() const noexcept { return tail->key; }
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front();
};

template <typename T>
forward_list<T>::~forward_list()
{
    node * temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void forward_list<T>::push_back(const T& value)
{
    if (head)
        tail = tail->next = new node {value, nullptr};
    else
        head = tail = new node {value, nullptr};
    ++var_size;
}

template <typename T>
void forward_list<T>::push_back(T&& value)
{
    if (head)
        tail = tail->next = new node {value, nullptr};
    else
        head = tail = new node {value, nullptr};
    ++var_size;
}

template <typename T>
void forward_list<T>::pop_front()
{
    // assume list non-empty
    node * temp = head;
    if (head == tail)
        head = nullptr;
    else
        head = head->next;
    delete temp;
    --var_size;
}

}

#endif // FORWARD_LIST_H_
