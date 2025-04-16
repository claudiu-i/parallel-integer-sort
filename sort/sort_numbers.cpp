#include <vector>
#include <iostream>
using namespace std;

// Merge two subarrays arr[l..m] and arr[m+1..r] to create sorted array arr[l..r]
void merge(vector<int> &arr, vector<int> &temp, int l, int m, int r)
{
    int i = l;
    int j = m + 1;
    int k = l; // index in main array

    while (i <= m && j <= r)
    {
        if (arr[i] <= arr[j])
        {
            temp[k] = arr[i];
            i++;
        }
        else
        {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

    while (i <= m)
    {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= r)
    {
        temp[k] = arr[j];
        j++;
        k++;
    }

    // Copy from temp back to main arr
    for (int idx = l; idx <= r; idx++)
    {
        arr[idx] = temp[idx];
    }
}

// Divide arr into two subarrays, sort, then merge them
void mergeSort(vector<int> &arr, vector<int> &temp, int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        mergeSort(arr, temp, l, m);
        mergeSort(arr, temp, m + 1, r);
        merge(arr, temp, l, m, r);
    }
}

int main()
{
    vector<int> arr;
    arr.reserve(1e6);
    int num;

    while (cin >> num)
    {
        arr.push_back(num);
    }

    vector<int> temp(arr.size()); // allocate temp array once
    mergeSort(arr, temp, 0, arr.size() - 1);

    for (int i : arr)
    {
        cout << i << "\n";
    }

    return 0;
}