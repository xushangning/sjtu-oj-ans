#include <iostream>

template <typename RandomIt>
void quick_sort(RandomIt first, RandomIt last)
{
    if (first < last) {
        auto p = *(last - 1);
        RandomIt i = first;
        decltype(p) temp;
        
        for (RandomIt j = first; j < last - 1; ++j)
            if (*j <= p) {
                // swap i and j
                temp = *i;
                *i = *j;
                *j = temp;
                ++i;
            }
        *(last - 1) = *i;
        *i = p;
        
        quick_sort(first, i);
        quick_sort(i + 1, last);
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);

    int n;
    std::cin >> n;
    int a[n];
    for (int i = 0; i < n; ++i)
        std::cin >> a[i];

    quick_sort(a, a + n);
    int count = 1;
    for (int i = 1, prev = a[0]; i < n; prev = a[i], ++i)
        if (prev != a[i])
            ++count;
    std::cout << count << std::endl;

    return 0;
}
