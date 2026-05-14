%%writefile hpc2a.cpp
#include <iostream>
#include <omp.h>
using namespace std;

void parallelBubbleSort(int *a, int n) {
    for (int i = 0; i < n; i++) {
        int first = i % 2;

        #pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void sequentialBubbleSort(int *b, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (b[j] > b[j + 1]) {
                int temp = b[j];
                b[j] = b[j + 1];
                b[j + 1] = temp;
            }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int *a = new int[n];
    int *b = new int[n];

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        b[i] = a[i];
    }

    double start, end;

    start = omp_get_wtime();
    sequentialBubbleSort(b, n);
    end = omp_get_wtime();
    cout << "\nSequential Bubble Sort Time: " << (end - start) << " seconds";

    start = omp_get_wtime();
    parallelBubbleSort(a, n);
    end = omp_get_wtime();
    cout << "\nParallel Bubble Sort Time: " << (end - start) << " seconds";

    cout << "\n\nSorted Array:\n";
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;

    delete[] a;
    delete[] b;
    return 0;
}

//command:
// !g++ hpc2a.cpp -fopenmp -o hpc2a
// !./hpc2a
