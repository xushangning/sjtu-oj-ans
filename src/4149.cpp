#include <iostream>

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
    node * head;
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
        operator const_iterator() { return p; }

        friend class circular_forward_list;
    };

    circular_forward_list() noexcept : var_size(0), head(nullptr) {}
    ~circular_forward_list();

    int size() const noexcept { return var_size; }
    bool empty() const noexcept { return !var_size; }
    iterator begin() noexcept { return head; }
    iterator insert(const_iterator pos, const T& value, iterator prev = nullptr);
    void erase(iterator pos, iterator prev = nullptr);
};

template <typename T>
circular_forward_list<T>::~circular_forward_list()
{
    if (head) {
        node * temp, * p = head->next;
        delete head;        // delete the head first
        // After deleting the head, delete the nodes after head until the list
        // circles back to head.
        while (p != head) {
            temp = p;
            p = p->next;
            delete temp;
        }
    }
}

/**
* prev: an iterator pointing to the node that is previous to the iterator pos,
* optionally supplied to eliminate the need for searching the list for a
* previous node of pos
*/
template <typename T>
typename circular_forward_list<T>::iterator
circular_forward_list<T>::insert(const_iterator pos, const T& value, iterator prev)
{
    if (empty()) {
        head->next = head = new node {value, nullptr};
        ++var_size;
        return head;
    }
    else {
        // if prev is not a node previous to pos
        if (prev.p == nullptr || prev.p->next != pos.p)
            // find the previous node of pos
            for (prev.p = head; prev.p->next != pos.p; ++prev);
        prev.p->next = new node {value, const_cast<node *>(pos.p)};
        ++var_size;
        return prev.p->next;
    }
}

/**
* prev: an iterator pointing to the node that is previous to the iterator pos,
* optionally supplied to eliminate the need for searching the list for a
* previous node of pos
*/
template <typename T>
void circular_forward_list<T>::erase(iterator pos, iterator prev)
{
    if (prev.p == nullptr || prev.p->next != pos.p)
        // find the previous node
        for (prev.p = head; prev.p->next != pos.p; ++prev);
    // assume a non-empty list as we are deleting nodes
    if (prev.p == pos.p)            // deleting the only remaining node
        head = nullptr;
    else {
        if (head == pos.p)       // deleting head
            head = pos.p->next;
        prev.p->next = pos.p->next;
    }
    --var_size;
    delete pos.p;
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    int n, m, k;
    std::cin >> n >> m >> k;
    circular_forward_list<int> l;
    circular_forward_list<int>::iterator iter = nullptr;
    for (int i = 1; i <= n; ++i)
        iter = l.insert(l.begin(), i, iter);
    // iter now points to the node labeled n, whose "next" points to l.begin().

    for (int i = 0; i < k; ++i) {
        // increment m times to reach the mth person
        // stops before the mth person
        for (int j = 0; j < m - 1; ++j)
            ++iter;
        circular_forward_list<int>::iterator temp = iter;
        ++temp;     // temp now points to the mth person
        if (i == k - 1)
            std::cout << *temp;
        else
            l.erase(temp, iter);
    }

    return 0;
}
