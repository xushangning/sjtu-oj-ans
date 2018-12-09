#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace sx
{

/*
 * select the last element as criterion
 * i, j: indices
 * init i to 0
 * for j in 0 to array size:
 *     if a[j] <= x:
 *         swap a[i] and a[j]
 *         increment i
 * swap a[i] and x
 * quick sort on the rest of the array
 */
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

}

#endif  // ALGORITHM_H
