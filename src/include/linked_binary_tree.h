#ifndef LINKED_BINARY_TREE_H
#define LINKED_BINARY_TREE_H

#include <iostream>
#include "queue.h"
#include "stack.h"

namespace sx
{

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

    enum class traversal_order {pre, in, post, level};
private:
    node * root;
    // whether the tree rooted at node * root is created with dynamic memory
    bool is_dynamic;

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
    * A level-order iterator for trees with more branches
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

    linked_binary_tree(node * root_) noexcept : root(root_), is_dynamic(false) {}
    template <typename RandomIt>
    linked_binary_tree(RandomIt preorder_first, RandomIt preorder_last, RandomIt inorder_first)
        : root(create_root(preorder_first, preorder_last, inorder_first)),
        is_dynamic(true) {}
    ~linked_binary_tree()
    {
        if (is_dynamic)
            erase(root);
    }

    iterator begin(traversal_order order = traversal_order::pre) { return iterator(root, order); }
    iterator& end(traversal_order order = traversal_order::pre) noexcept;

    bool complete() const;

    template <typename RandomIt>
    static node * create_root(RandomIt preorder_first, RandomIt preorder_last,
                              RandomIt inorder_first);
    static node * link_nodes(node * nodes, int arr_size, bool read_key = true);
    static void erase(node * n);
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
* Check whether the binary tree is complete.
*/
template <typename T>
bool linked_binary_tree<T>::complete() const
{
    queue<const node *> q;
    // will be set to false once a 0 or 1 degree node is encountered
    bool no_leaf_node = true;
    q.push(root);

    const node * n;
    while (!q.empty()) {
        n = q.front();
        q.pop();
        // if the node has only a right child
        if (n->left == nullptr && n->right)
            return false;
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
typename linked_binary_tree<T>::node * linked_binary_tree<T>::create_root(
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

/**
* Free memory allocated to nodes in a postorder fashion.
*/
template <typename T>
void linked_binary_tree<T>::erase(node * n)
{
    if (n->left)
        erase(n->left);
    if (n->right)
        erase(n->right);
    delete n;
}

}

#endif // LINKED_BINARY_TREE_H
