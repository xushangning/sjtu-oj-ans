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
private:
    node * root;
    // whether the tree rooted at node * root is created with dynamic memory
    bool is_dynamic;

    /**
    * An iterator class that iterates on nodes
    */
    class node_iterator
    {
    private:
        stack<node *> s;
    public:
        node_iterator() = default;
        node_iterator(node * root) { s.push(root); }

        node_iterator& operator++();
        node * operator*() const noexcept { return s.top(); }
        bool operator==(const node_iterator& ni) const noexcept
        {
            // Two iterators are equal if and only if:
            // 1. their stacks both are empty;
            // 2. their stacks have the same element at top.
            return s.empty() && ni.s.empty() ||
                (!s.empty() && !ni.s.empty() && s.top() == ni.s.top());
        }
        bool operator!=(const node_iterator& ni) const noexcept { return !(*this == ni); }
    };
public:
    /**
    * An iterator class that iterates on keys
    */
    class iterator : private node_iterator
    {
    private:
        static iterator end_iter;   // for signaling past-the-end
    public:
        iterator() = default;
        iterator(node * root) : node_iterator(root) {}

        iterator& operator++()
        {
            node_iterator::operator++();
            return *this;
        }
        T& operator*() const noexcept { return node_iterator::operator*()->key; }
        bool operator==(const iterator& i) const noexcept
        {
            return node_iterator::operator==(dynamic_cast<const node_iterator&>(i));
        }
        bool operator!=(const iterator& i) const noexcept { return !(*this == i); }

        friend class linked_binary_tree;
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

    iterator begin() { return root; }
    iterator end() { return iterator::end_iter; }

    bool complete() const;

    template <typename RandomIt>
    static node * create_root(RandomIt preorder_first, RandomIt preorder_last,
                              RandomIt inorder_first);
    static node * link_nodes(node * nodes, int arr_size, bool read_key = true);
    static void erase(node * n);
};

template <typename T>
typename linked_binary_tree<T>::node_iterator&
linked_binary_tree<T>::node_iterator::operator++()
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
typename linked_binary_tree<T>::iterator linked_binary_tree<T>::iterator::end_iter;

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
