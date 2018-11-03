#include <iostream>
#include <cstring>

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
    bool empty() const { return c.empty(); }
    int size() const { return c.size(); }
    void push(const T& value) { c.push_back(value); }
    void push(T&& value) { c.push_back(value); }
    void pop() { c.pop_front(); }
};

template <typename T>
class binary_tree
{
public:
    class node
    {
    private:
        T key;
        node * left, * right;
        int pos;    // position of the node in level walking the tree
        node(const T& key_, node * left_ = nullptr, node * right_ = nullptr) noexcept
            : key(key_), left(left_), right(right_) {}

        friend class binary_tree;
    };
private:
    node * root;
public:
    binary_tree(node * root_) noexcept : root(root_) {}
    template <typename RandomIt>
    binary_tree(RandomIt preorder_first, RandomIt preorder_last, RandomIt inorder_first)
        : root(create_root(preorder_first, preorder_last, inorder_first)) {}

    template <typename RandomIt>
    static node * create_root(RandomIt preorder_first, RandomIt preorder_last,
                              RandomIt inorder_first);

    void print(int tree_size) const;
};

/**
* Construct a tree rooted at the returned node from its preorder and inorder
* walk.
*
* preorder_first, preorder_last: the preorder walk of the tree
* inorder_first: the inorder walk of the tree, assuming that the array has the
* same size
*/
template <typename T>
    template <typename RandomIt>
typename binary_tree<T>::node * binary_tree<T>::create_root(
        RandomIt preorder_first, RandomIt preorder_last, RandomIt inorder_first)
{
    // create the root from the first element of the preorder walk
    node * root = new node(*preorder_first);
    RandomIt root_pos;
    // find the position of the root in the inorder walk
    for (root_pos = inorder_first; *root_pos != root->key; ++root_pos);

    // divide the inorder walk array into left and right
    int left_size = root_pos - inorder_first;
    if (left_size == 1)
        root->left = new node(*inorder_first);
    else if (left_size)
        root->left = create_root(
            preorder_first + 1, preorder_first + left_size + 1, inorder_first);
    // for the right part
    int right_size = inorder_first + (preorder_last - preorder_first) - root_pos - 1;
    if (right_size == 1)
        root->right = new node(*(root_pos + 1));
    else if (right_size)
        root->right = create_root(
            preorder_first + left_size + 1, preorder_last, root_pos + 1);

    return root;
}

template <typename T>
void binary_tree<T>::print(int tree_size) const
{
    queue<const node *> q;
    const node * output[1000];
    for (int i = 0; i < 1000; ++i)
        output[0] = nullptr;

    root->pos = 0;
    q.push(root);
    const node * n;
    while (!q.empty()) {
        n = q.front();
        q.pop();
        output[n->pos] = n;
        if (n->left) {
            n->left->pos = 2 * n->pos + 1;
            q.push(n->left);
        }
        if (n->right) {
            n->right->pos = 2 * n->pos + 2;
            q.push(n->right);
        }
    }

    int n_node_printed = 0;
    for (int i = 0; n_node_printed < tree_size; ++i) {
        if (output[i]) {
            std::cout << output[i]->key;
            ++n_node_printed;
        }
        else
            std::cout << "NULL";
        if (n_node_printed != tree_size)
            std::cout.put(' ');
    }
    std::cout.put('\n');
}

int main()
{
    char preorder[27], inorder[27];
    std::cin >> preorder >> inorder;
    int tree_size = std::strlen(preorder);
    binary_tree<char> t(preorder, preorder + tree_size, inorder);
    t.print(tree_size);
    return 0;
}
