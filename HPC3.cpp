#include <iostream>
#include <omp.h>
#include <climits>
using namespace std;

// Function to calculate the minimum value using parallel reduction
void min_reduction(int arr[], int n) {
  int min_value = INT_MAX;  // Initialize min_value to the largest possible integer
  #pragma omp parallel for reduction(min: min_value)  // Parallelize the loop with reduction on min_value
  for (int i = 0; i < n; i++) {
    if (arr[i] < min_value) {
      min_value = arr[i];  // Update the minimum value
    }
  }
  cout << "Minimum value: " << min_value << endl;
}

// Function to calculate the maximum value using parallel reduction
void max_reduction(int arr[], int n) {
  int max_value = INT_MIN;  // Initialize max_value to the smallest possible integer
  #pragma omp parallel for reduction(max: max_value)  // Parallelize the loop with reduction on max_value
  for (int i = 0; i < n; i++) {
    if (arr[i] > max_value) {
      max_value = arr[i];  // Update the maximum value
    }
  }
  cout << "Maximum value: " << max_value << endl;
}

// Function to calculate the sum of elements using parallel reduction
void sum_reduction(int arr[], int n) {
  int sum = 0;  // Initialize sum to 0
  #pragma omp parallel for reduction(+: sum)  // Parallelize the loop with reduction on sum
  for (int i = 0; i < n; i++) {
    sum += arr[i];  // Add each element to sum
  }
  cout << "Sum: " << sum << endl;
}

// Function to calculate the average of elements using parallel reduction
void average_reduction(int arr[], int n) {
  int sum = 0;  // Initialize sum to 0
  #pragma omp parallel for reduction(+: sum)  // Parallelize the loop with reduction on sum
  for (int i = 0; i < n; i++) {
    sum += arr[i];  // Add each element to sum
  }
  cout << "Average: " << (double)sum / n << endl;  // Calculate and print the average
}

int main() {
    int *arr, n;

    // Take input for number of elements
    cout << "\nEnter total number of elements: ";
    cin >> n;

    // Dynamically allocate memory for the array
    arr = new int[n];

    // Take input for array elements
    cout << "\nEnter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Call the functions to perform min, max, sum, and average operations
    min_reduction(arr, n);
    max_reduction(arr, n);
    sum_reduction(arr, n);
    average_reduction(arr, n);

    // Free dynamically allocated memory
    delete[] arr;

    return 0;
}
