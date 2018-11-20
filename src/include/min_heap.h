#ifndef MIN_HEAP_H_INCLUDED
#define MIN_HEAP_H_INCLUDED

#include "vector.h"

namespace sx
{

template <typename T> class min_heap;

template <typename T>
std::ostream &operator<<(std::ostream &os, const min_heap<T> &h);

template <typename T>
class min_heap
{
private:
    vector<T> v;

    void heapify(int i) noexcept;

    static int left(int i) noexcept { return 2 * i + 1; }
    static int right(int i) noexcept { return 2 * i + 2; }
    static int parent(int i) noexcept { return i ? (i - 1) / 2 : -1; }
public:
    static const int NOT_FOUND;

    const T& top() const noexcept { return v.front(); }
    T& top() noexcept { return v.front(); }
    void push(const T& value);

    /**
    * Find a place for node i whose key may be less than its parent. It
    * repeatedly compares the key with its parent, effectively "floats up"
    * the node.
    */
    void float_up(int i) noexcept;

    const T& operator[](int i) const noexcept { return v[i]; }

    friend std::ostream &operator<< <>(std::ostream &os, const min_heap &h);
};

template <typename T>
void min_heap<T>::heapify(int i) noexcept
{
    // record the least element between node i and its two children
    int least = i;
    int l = left(i);
    int r;
    // loop as long as the node has children
    while (l < v.size()) {
        if (v[least] > v[l])
            least = l;
        r = right(i);
        if (r < v.size() && v[least] > v[r])
            least = r;
        if (least == i)     // the element at node i is the least
            break;
        else {
            T temp = v[i];
            v[i] = v[least];
            v[least] = temp;
            i = least;
        }
        l = left(i);
    }
}

template <typename T>
void min_heap<T>::push(const T& value)
{
    v.push_back(value);
    float_up(v.size() - 1);
}

template <typename T>
inline void min_heap<T>::float_up(int i) noexcept
{
    int p = parent(i);
    // manually handle the first swap
    if (p >= 0 && v[p] > v[i]) {
        // optimise swapping by creating a persistent variable temp instead of
        // many temporary ones
        T temp = v[i];
        v[i] = v[p];    // assign the parent's key to the node
        i = p;
        p = parent(i);

        // compare v[p] with temp but not v[i] because the key stored in the
        // variable temp hasn't been assigned to any node
        while (p >= 0 && v[p] > temp) {
            v[i] = v[p];
            i = p;
            p = parent(i);
        }

        v[i] = temp;
    }
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const min_heap<T> &h)
{
    os << h.v;
    return os;
}

}

#endif // MIN_HEAP_H_INCLUDED
