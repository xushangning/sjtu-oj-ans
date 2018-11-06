#include <iostream>

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

template <typename T>
class linked_binary_tree
{
public:
    class node
    {
    private:
        T key;
        node * p, * left, * right;
    public:
        node() noexcept : p(nullptr), left(nullptr), right(nullptr) {}
        node(const T& key_, node * left_ = nullptr, node * right_ = nullptr) noexcept
            : key(key_), left(left_), right(right_) {}

        int degree() const noexcept { return bool(left) + bool(right); }

        friend class linked_binary_tree;
    };

    enum class traversal_order {pre, in, level};
private:
    node * root;

    /**
    * An interface for iterators that iterate on nodes
    */
    class node_iterator
    {
    public:
        virtual ~node_iterator() = default;

        virtual node_iterator& operator++() = 0;
        virtual node * operator*() const noexcept = 0;
    };

    /**
    * A preorder iterator
    */
    class node_iterator_preorder : public node_iterator
    {
    private:
        stack<node *> s;
    public:
        node_iterator_preorder(node * root)
        {
            if (root)
                s.push(root);
        }

        node_iterator_preorder& operator++();
        node * operator*() const noexcept { return s.top(); }
        bool operator==(const node_iterator_preorder& ni) const noexcept
        {
            // Two preorder iterators are equal if and only if
            // their stacks both are empty.
            return s.empty() && ni.s.empty();
        }
    };

    class node_iterator_inorder : public node_iterator
    {
    private:
        stack<node *> s;
    public:
        node_iterator_inorder(node * root);

        node * operator*() const noexcept { return s.top(); }
        node_iterator_inorder& operator++();
        bool operator==(const node_iterator_inorder& ni) const noexcept
        {
            // Two inorder iterators are equal if and only if
            // their stacks both are empty.
            return s.empty() && ni.s.empty();
        }
    };

    /**
    * A level-order iterator for trees with more than two branches
    */
    class node_iterator_levelorder : public node_iterator
    {
    private:
        queue<node *> q;
    public:
        node_iterator_levelorder(node * root)
        {
            if (root)
                q.push(root);
        }

        node * operator*() const noexcept { return q.front(); }
        node_iterator_levelorder& operator++();
        bool operator==(const node_iterator_levelorder& ni) const noexcept
        {
            // Two level-order iterators are equal if and only if
            // their queues both are empty.
            return q.empty() && ni.q.empty();
        }
    };
public:
    /**
    * An iterator class that iterates on keys. Internally, there is a pointer
    * to node_iterator, ni, that decides which node_iterator to use. The
    * iterator iterates on nodes and returns keys if deferenced.
    *
    * Static iterators like preorder_end indicate past-the-end. They are
    * returned by reference by linked_binary_tree::end().
    */
    class iterator
    {
    private:
        traversal_order order;
        node_iterator * ni;
    public:
        iterator(node * root, traversal_order order_ = traversal_order::pre);
        iterator(const iterator& i);
        ~iterator() { delete ni; }

        iterator& operator++()
        {
            ++*ni;
            return *this;
        }
        T& operator*() const noexcept { return (**ni)->key; }
        bool operator==(const iterator& i) const noexcept;
        bool operator!=(const iterator& i) const noexcept { return !(*this == i); }

        // for indicating past-the-end
        static iterator preorder_end;
        static iterator inorder_end;
        static iterator levelorder_end;
    };

    linked_binary_tree(node * root_) noexcept : root(root_) {}

    iterator begin(traversal_order order = traversal_order::pre) { return iterator(root, order); }
    iterator& end(traversal_order order = traversal_order::pre) noexcept;

    static node * link_nodes(node * nodes, int arr_size, bool read_key = true);
};

template <typename T>
typename linked_binary_tree<T>::node_iterator_preorder&
linked_binary_tree<T>::node_iterator_preorder::operator++()
{
    node * n = s.top();
    s.pop();
    if (n->right)
        s.push(n->right);
    if (n->left)
        s.push(n->left);
    return *this;
}

template <typename T>
linked_binary_tree<T>::node_iterator_inorder::node_iterator_inorder(node * root)
{
    // find and put the leftmost node at top
    while (root) {
        s.push(root);
        root = root->left;
    }
}

template <typename T>
typename linked_binary_tree<T>::node_iterator_inorder&
linked_binary_tree<T>::node_iterator_inorder::operator++()
{
    // The left subtree of the node at the top of the stack must have been
    // traversed, so we only deal with the right subtree.
    node * n = s.top();
    s.pop();
    // if right not null, traverse the right subtree
    if (n->right) {
        s.push(n = n->right);
        while (n->left)     // find the leftmost node in the right subtree
            s.push(n = n->left);
    }
    // else, having popped the node, get back to its parent
    return *this;
}

template <typename T>
typename linked_binary_tree<T>::node_iterator_levelorder&
linked_binary_tree<T>::node_iterator_levelorder::operator++()
{
    node * n = q.front();
    if (n->left)
        q.push(n->left);        // enqueue next level
    if (n->right)
        q.front() = n->right;   // modify the front element
    else
        q.pop();                // go down one level
    return *this;
}

template <typename T>
linked_binary_tree<T>::iterator::iterator(node * root, traversal_order order_)
        : order(order_)
{
    switch (order)
    {
        case traversal_order::pre:
            ni = new node_iterator_preorder(root);
            break;
        case traversal_order::in:
            ni = new node_iterator_inorder(root);
            break;
        case traversal_order::level:
            ni = new node_iterator_levelorder(root);
            break;
    }
}

template <typename T>
linked_binary_tree<T>::iterator::iterator(const iterator& i) : order(i.order)
{
    switch (order)
    {
        case traversal_order::pre:
            ni = new node_iterator_preorder(* dynamic_cast<const node_iterator_preorder *>(i.ni));
            break;
        case traversal_order::in:
            ni = new node_iterator_inorder(* dynamic_cast<const node_iterator_inorder *>(i.ni));
            break;
        case traversal_order::level:
            ni = new node_iterator_levelorder(* dynamic_cast<const node_iterator_levelorder *>(i.ni));
            break;
    }
}

template <typename T>
bool linked_binary_tree<T>::iterator::operator==(const iterator& i) const noexcept
{
    if (order != i.order)
        return false;
    else
        switch (order)
        {
            case traversal_order::pre:
                return * dynamic_cast<const node_iterator_preorder *>(ni)
                    == * dynamic_cast<const node_iterator_preorder *>(i.ni);
            case traversal_order::in:
                return * dynamic_cast<const node_iterator_inorder *>(ni)
                    == * dynamic_cast<const node_iterator_inorder *>(i.ni);
            case traversal_order::level:
                return * dynamic_cast<const node_iterator_levelorder *>(ni)
                    == * dynamic_cast<const node_iterator_levelorder *>(i.ni);
        }
}

template <typename T>
typename linked_binary_tree<T>::iterator
linked_binary_tree<T>::iterator::preorder_end(nullptr);

template <typename T>
typename linked_binary_tree<T>::iterator
linked_binary_tree<T>::iterator::inorder_end(nullptr, traversal_order::in);

template <typename T>
typename linked_binary_tree<T>::iterator
linked_binary_tree<T>::iterator::levelorder_end(nullptr, traversal_order::level);

template <typename T>
typename linked_binary_tree<T>::iterator&
linked_binary_tree<T>::end(traversal_order order) noexcept
{
    switch (order)
    {
        case traversal_order::pre:
            return iterator::preorder_end;
        case traversal_order::in:
            return iterator::inorder_end;
        case traversal_order::level:
            return iterator::levelorder_end;
    }
}

/**
* Link nodes in an array together to form a tree based on input in the form
*     <node-index> <node-index> <key>
* from standard input.
*
* nodes: the array of nodes
* arr_size: the size of the array
* read_key: read <key> if true. true by default.
*
* return: the root in the array of nodes
*/
template <typename T>
typename linked_binary_tree<T>::node *
linked_binary_tree<T>::link_nodes(node * nodes, int arr_size, bool read_key)
{
    int left, right;
    // <node-index> == 0 for no child
    //              == i (i = 1, ..., arr_size) for node[i]
    if (read_key)
        for (int i = 0; i < arr_size; ++i) {
            std::cin >> left >> right >> nodes[i].key;
            if (left) {
                nodes[i].left = &nodes[left - 1];
                nodes[left - 1].p = &nodes[i];
            }
            if (right) {
                nodes[i].right = &nodes[right - 1];
                nodes[right - 1].p = &nodes[i];
            }
        }
    else
        for (int i = 0; i < arr_size; ++i) {
            std::cin >> left >> right;
            if (left) {
                nodes[i].left = &nodes[left - 1];
                nodes[left - 1].p = &nodes[i];
            }
            if (right) {
                nodes[i].right = &nodes[right - 1];
                nodes[right - 1].p = &nodes[i];
            }
        }

    // find the root in the array of nodes
    node * root;
    for (root = nodes; root->p; root = root->p);
    return root;
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    int tree_size;
    std::cin >> tree_size;
    linked_binary_tree<int>::node nodes[tree_size];
    linked_binary_tree<int> t(linked_binary_tree<int>::link_nodes(nodes, tree_size));

    for (linked_binary_tree<int>::iterator i = t.begin(); i != t.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    for (linked_binary_tree<int>::iterator i = t.begin(linked_binary_tree<int>::traversal_order::in);
            i != t.end(linked_binary_tree<int>::traversal_order::in); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    for (linked_binary_tree<int>::iterator i = t.begin(linked_binary_tree<int>::traversal_order::level);
            i != t.end(linked_binary_tree<int>::traversal_order::level); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;

    return 0;
}
