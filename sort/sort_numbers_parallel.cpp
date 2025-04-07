#include <iostream>
#include <vector>
#include <thread>
#include <future> // for std::async
using namespace std;

// Parameters to limit thread creation for small tasks
int threshold = 5000;
int maxDepth = 3; // log2(num_cores)

// Merge two subarrays arr[l..m] and arr[m+1..r] to create sorted array arr[l..r]
void merge(vector<int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }

    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Use two threads to merge simulatenously
void doubleMerge(vector<int> &arr, int l, int m, int r)
{
    int n = r - l + 1;
    vector<int> temp(n); // array for merged result

    int leftSize = m - l + 1;
    int rightSize = r - m;

    // Merges the minimum elements from the beginning
    auto mergeMins = [&]()
    {
        int i = l;     // index for left subarray
        int j = m + 1; // index for right subarray
        int k = 0;     // index for temp (starts at beginning)
        while (k < n / 2)
        {
            if (j > r || (i <= m && arr[i] <= arr[j]))
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
    };

    // Merges the maximum elements from the end
    auto mergeMaxes = [&]()
    {
        int i = m;
        int j = r;
        int k = n - 1; // index for temp (starts at end)
        while (k >= n / 2)
        {
            if (j < m + 1 || (i >= l && arr[i] > arr[j]))
            {
                temp[k] = arr[i];
                i--;
            }
            else
            {
                temp[k] = arr[j];
                j--;
            }
            k--;
        }
    };

    // Launch one async thread to merge two halves in parallel
    auto future = async(launch::async, mergeMins);
    mergeMaxes();
    future.wait();

    // Copy merged result back into main array
    for (int i = 0; i < n; i++)
    {
        arr[l + i] = temp[i];
    }
}

// (Serial) Divide arr into two subarrays, sort, then merge them
void mergeSort(vector<int> &arr, int l, int r)
{
    if (l < r)
    {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// (Parallel) Divide arr into two subarrays, sort, then merge them
void parallelMergeSort(vector<int> &arr, int l, int r, int depth = 0)
{
    if (l < r)
    {
        int m = (l + r) / 2;

        if ((r - l) > threshold && depth < maxDepth)
        {
            // Launch one async thread to sort left and right in parallel
            auto leftFuture = async(launch::async, parallelMergeSort, ref(arr), l, m, depth + 1);
            parallelMergeSort(arr, m + 1, r, depth + 1); // sort right
            leftFuture.wait();
        }
        else
        {
            // Default to serial implementation if sorting task too small
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
        }

        // Double merging
        doubleMerge(arr, l, m, r);
    }
}

// Driver function
int main()
{
    vector<int> arr;
    int num;

    while (cin >> num)
    {
        arr.push_back(num);
    }

    parallelMergeSort(arr, 0, arr.size() - 1);

    for (int i : arr)
    {
        cout << i << "\n";
    }
}
