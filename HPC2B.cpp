%%writefile hpc2b.cpp
#include <iostream>
#include <omp.h>
using namespace std;

void merge(int a[], int i1, int j1, int i2, int j2);

void mergesort_parallel(int a[], int i, int j) {
    int mid;
    if (i < j) {
        mid = (i + j) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            mergesort_parallel(a, i, mid);  // Sort left half

            #pragma omp section
            mergesort_parallel(a, mid + 1, j);  // Sort right half
        }

        merge(a, i, mid, mid + 1, j);  // Merge sorted halves
    }
}

void mergesort_sequential(int a[], int i, int j) {
    int mid;
    if (i < j) {
        mid = (i + j) / 2;
        mergesort_sequential(a, i, mid);  // Sort left half
        mergesort_sequential(a, mid + 1, j);  // Sort right half
        merge(a, i, mid, mid + 1, j);  // Merge sorted halves
    }
}

void merge(int a[], int i1, int j1, int i2, int j2) {
    int temp[1000];  // Temporary array for merging
    int i = i1, j = i2, k = 0;

    // Merge two sorted parts
    while (i <= j1 && j <= j2) {
        if (a[i] < a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    // Copy remaining elements
    while (i <= j1)
        temp[k++] = a[i++];

    while (j <= j2)
        temp[k++] = a[j++];

    // Copy merged array back to original array
    for (i = i1, j = 0; i <= j2; i++, j++)
        a[i] = temp[j];
}

int main() {
    int *a, *b, n;
    cout << "Enter total number of elements: ";
    cin >> n;

    a = new int[n];
    b = new int[n];

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        b[i] = a[i];  // Copy array for sequential sorting
    }

    double start, end;

    // Sequential Merge Sort
    start = omp_get_wtime();
    mergesort_sequential(b, 0, n - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << (end - start) << " seconds\n";

    // Parallel Merge Sort
    start = omp_get_wtime();
    mergesort_parallel(a, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << (end - start) << " seconds\n";

    // Display sorted array
    cout << "\nSorted array is: ";
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;

    delete[] a;
    delete[] b;

    return 0;
}

!g++ hpc2b.cpp -fopenmp -o hpc2b
!./hpc2b
