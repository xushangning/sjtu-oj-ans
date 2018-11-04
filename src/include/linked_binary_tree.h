#ifndef LINKED_BINARY_TREE_H
#define LINKED_BINARY_TREE_H

#include "queue.h"

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
        node * left, * right;
    public:
        node() noexcept : left(nullptr), right(nullptr) {}
        node(const T& key_, node * left_ = nullptr, node * right_ = nullptr) noexcept
            : key(key_), left(left_), right(right_) {}

        friend class linked_binary_tree;
    };
private:
    node * root;
    // whether the tree rooted at node * root is created with dynamic memory
    bool is_dynamic;
public:
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

    bool complete() const;

    template <typename RandomIt>
    static node * create_root(RandomIt preorder_first, RandomIt preorder_last,
                              RandomIt inorder_first);
    static void erase(node * n);
};

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
