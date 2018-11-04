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
    forward_list() noexcept : var_size(0), head(nullptr), tail(nullptr) {}
    ~forward_list();

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
    void push(const T& value) { c.push_back(value); }
    void push(T&& value) { c.push_back(value); }
    void pop() { c.pop_front(); }
};

class linked_binary_tree
{
public:
    struct node
    {
        node * p, * left, * right;

        node() noexcept : p(nullptr), left(nullptr), right(nullptr) {}

        int degree() const noexcept { return bool(left) + bool(right); }
    };
private:
    node * root;
public:
    linked_binary_tree(node * root_) noexcept : root(root_) {}

    bool complete() const;
};

bool linked_binary_tree::complete() const
{
    queue<const node *> q;
    bool no_leaf_node = true;
    q.push(root);

    const node * n;
    while (!q.empty()) {
        n = q.front();
        q.pop();
        // The case where a node only has a right children has been checked at
        // initialization.
        if (no_leaf_node) {
            if (n->degree() < 2)
                no_leaf_node = false;
        }
        else if (n->degree())
            return false;
        if (n->left)
            q.push(n->left);
        if (n->right)
            q.push(n->right);
    }
    return true;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int tree_size;
    std::cin >> tree_size;
    linked_binary_tree::node nodes[tree_size];
    bool is_cbt = true;

    int left, right;
    for (int i = 0; i < tree_size; ++i) {
        std::cin >> left >> right;
        if (left == 0 && right) {
            is_cbt = false;
            break;
        }
        if (left) {
            nodes[i].left = &nodes[left - 1];
            nodes[left - 1].p = &nodes[i];
        }
        if (right) {
            nodes[i].right = &nodes[right - 1];
            nodes[right - 1].p = &nodes[i];
        }
    }

    if (is_cbt) {
        linked_binary_tree::node * root;
        // find the root
        for (root = &nodes[tree_size - 1]; root->p; root = root->p);
        linked_binary_tree t(root);
        is_cbt = t.complete();
    }
    std::cout.put(is_cbt ? 'Y' : 'N');

    return 0;
}
