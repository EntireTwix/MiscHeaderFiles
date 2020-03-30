#pragma once
#include <algorithm>
#include <vector>


template <typename T>
class ISortable {
    virtual bool operator<(const T& instance) = 0;
    virtual bool operator>(const T& instance) = 0;
};

    template <typename T>
    int Partition(std::vector<T>& arr, int low, int high)
    {
        T pivot = arr[high]; // pivot  
        int i = (low - 1); // Index of smaller element  

        for (int j = low; j <= high - 1; j++)
        {
            // If current element is smaller than the pivot  
            if (arr[j] > pivot)
            {
                i++; // increment index of smaller element  
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

    template <typename T>
    void QuickSort(std::vector<T>& arr, int low, int high)
    {
        if (low < high)
        {
            /* pi is partitioning index, arr[p] is now
            at right place */
            int pi = Partition(arr, low, high);

            // Separately sort elements before  
            // partition and after partition  
            QuickSort(arr, low, pi - 1);
            QuickSort(arr, pi + 1, high);
        }
    }
