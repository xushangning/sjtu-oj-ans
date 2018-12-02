#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

namespace sx
{

template <typename T> class vector;

template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& v);

template <typename T>
class vector
{
private:
    int var_size, var_capacity;
    T * data;
    void expand(int target = -1);
public:
    typedef T * iterator;
    typedef const T * const_iterator;

    vector(int var_size_ = 0);
    vector(const vector & v);
    vector(vector && v) noexcept
        : var_size(v.var_size), var_capacity(v.var_capacity), data(v.data)
    {
        v.data = nullptr;
    }
    const vector & operator=(const vector & v);
    const vector & operator=(vector && v) noexcept;
    ~vector() { delete []data; }

    iterator begin() noexcept { return data; }
    const_iterator begin() const noexcept { return data; }
    iterator end() noexcept { return data + var_size; }
    const_iterator end() const noexcept { return data + var_size; }
    T& front() noexcept { return *data; }
    const T& front() const noexcept { return *data; }
    T& back() noexcept { return data[var_size - 1]; }
    const T& back() const noexcept { return data[var_size - 1]; }
    int size() const noexcept { return var_size; }
    int capacity() const noexcept { return var_capacity; }

    iterator insert(iterator pos, const T & value);
    template <typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator erase(const_iterator pos);

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_front() { erase(begin()); }

    const T & operator[](int i) const noexcept { return data[i]; }
    T & operator[](int i) noexcept { return data[i]; }
    vector operator+(const vector & v) const;

    friend std::ostream& operator<< <>(std::ostream& os, const vector& v);
};

template <typename T>
void vector<T>::expand(int target)
{
    T * old_data = data;
    if (target == -1) {
        if (var_capacity)
            var_capacity *= 2;      // by default double space
        else
            var_capacity = 1;
    }
    else if (var_capacity)
        while ((var_capacity *= 2) < target);
    else
        var_capacity = target;
    data = new T [var_capacity];
    for (int i = 0; i < var_size; ++i)
        data[i] = old_data[i];
    delete []old_data;
}

template <typename T>
vector<T>::vector(int var_size_) : var_size(var_size_),
        var_capacity(var_size_)
{
    if (var_size > 0)
        data = new T [var_size];
    else
        data = nullptr;
}

template <typename T>
vector<T>::vector(const vector & v)
        : var_size(v.var_size), var_capacity(v.var_size)
{
    if (var_size) {
        data = new T [var_size];
        for (int i = 0; i < var_size; ++i)
            data[i] = v.data[i];
    }
    else
        data = nullptr;
}

template <typename T>
const vector<T> & vector<T>::operator=(const vector & v)
{
    if (this == &v)
        return *this;
    delete []data;
    v.var_size = v.var_capacity = v.var_size;
    if (var_size) {
        data = new T [var_size];
        for (int i = 0; i < var_size; ++i)
            data[i] = v.data[i];
    }
    else
        data = nullptr;
    return *this;
}

template <typename T>
const vector<T> & vector<T>::operator=(vector && v) noexcept
{
    if (this == &v)
        return *this;
    delete []data;
    v.var_size = v.var_capacity = v.var_size;
    data = v.data;
    v.data = nullptr;
    return *this;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos, const T & value)
{
    // pos will be invalidated if space is reallocated
    int offset = pos - begin();
    if (var_size + 1 > var_capacity)
        expand();
    for (iterator i = end(); i > begin() + offset; --i)
        *i = *(i - 1);
    iterator value_pos = begin() + offset;
    *value_pos = value;
    ++var_size;
    return value_pos;
}

template <typename T>
    template <typename InputIt>
typename vector<T>::iterator vector<T>::insert(const_iterator pos,
                                      InputIt first, InputIt last)
{
    int offset = pos - begin();
    int insertion_size = last - first;
    if (insertion_size + var_size > var_capacity)
        expand(insertion_size + var_size);
    iterator new_pos = begin() + offset;
    for (iterator i = end() - 1; i >= new_pos; --i)
        *(i + insertion_size) = *i;
    InputIt j = first;
    for (iterator i = new_pos; j < last; ++i, ++j)
        *i = *j;
    var_size += insertion_size;
    return new_pos;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos)
{
    for (iterator i = const_cast<iterator>(pos); i + 1 < end(); ++i)
        *i = *(i + 1);
    --var_size;
    return const_cast<iterator>(++pos);
}

template <typename T>
void vector<T>::push_back(const T& value)
{
    if (var_size + 1 > var_capacity)
        expand();
    *end() = value;
    ++var_size;
}

template <typename T>
void vector<T>::push_back(T&& value)
{
    if (var_size + 1 > var_capacity)
        expand();
    *end() = value;
    ++var_size;
}

template <typename T>
vector<T> vector<T>::operator+(const vector & v) const
{
    vector result(*this);
    result.insert(result.end(), v.begin(), v.end());
    return result;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& v)
{
    typename vector<T>::const_iterator i = v.begin();
    for (; i < v.end() - 1; ++i)
        os << *i << ' ';
    if (i < v.end())
        os << *i;
    return os;
}

}

#endif // VECTOR_H_
