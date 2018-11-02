#include <iostream>

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
    ~vector() { delete []data; }

    iterator begin() noexcept { return data; }
    const_iterator begin() const noexcept { return data; }
    iterator end() noexcept { return data + var_size; }
    const_iterator end() const noexcept { return data + var_size; }
    int size() const noexcept { return var_size; }
    int capacity() const noexcept { return var_capacity; }

    iterator insert(iterator pos, const T & value);
    iterator erase(const_iterator pos);

    void push_back(const T& value) { insert(end(), value); }
    void push_back(T&& value) { insert(end(), value); }
    void pop_front() { erase(begin()); }
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
typename vector<T>::iterator vector<T>::erase(const_iterator pos)
{
    for (iterator i = const_cast<iterator>(pos); i + 1 < end(); ++i)
        *i = *(i + 1);
    --var_size;
    return const_cast<iterator>(++pos);
}

template <typename T, typename Container>
class queue
{
protected:
    Container c;
public:
    int size() const { return c.size(); }
    void push(const T& value) { c.push_back(value); }
    void push(T&& value) { c.push_back(value); }
    void pop() { c.pop_front(); }

    T average() const noexcept;
};

template <typename T, typename Container>
T queue<T, Container>::average() const noexcept
{
    T sum = 0;
    for (const T& x : c)
        sum += x;
    return sum / size();
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    queue<int, vector<int>> q;
    int n, m;
    std::cin >> n >> m;
    int temp;
    for (int i = 0; i < m; ++i) {
        std::cin >> temp;
        if (q.size() == n)
            q.pop();
        q.push(temp);
        std::cout << q.average() << std::endl;
    }

    return 0;
}
