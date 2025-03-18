#include <iostream>
#include <vector>
#include <omp.h>
#include <limits>
#include <chrono>

using namespace std;
using namespace chrono;

float serialReduction(const vector<int> &arr) {
    auto start_time = high_resolution_clock::now();
    int minVal = numeric_limits<int>::max();
    int maxVal = numeric_limits<int>::min();
    long long sum = 0;
    int n = arr.size();
    
    for (int i = 0; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
        sum += arr[i];
    }
    
    double average = static_cast<double>(sum) / n;
    auto end_time = high_resolution_clock::now();
    
    cout << "Serial Execution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds" << endl;
    cout << "Serial Minimum: " << minVal << endl;
    cout << "Serial Maximum: " << maxVal << endl;
    cout << "Serial Sum: " << sum << endl;
    cout << "Serial Average: " << average << endl;
    return (end_time - start_time).count();
}

float parallelReduction(const vector<int> &arr) {
    auto start_time = high_resolution_clock::now();
    int minVal = numeric_limits<int>::max();
    int maxVal = numeric_limits<int>::min();
    long long sum = 0;
    int n = arr.size();
    
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if (arr[i] < minVal) minVal = arr[i];
        if (arr[i] > maxVal) maxVal = arr[i];
        sum += arr[i];
    }
    
    double average = static_cast<double>(sum) / n;
    auto end_time = high_resolution_clock::now();
    
    cout << "Parallel Execution Time: " << duration_cast<microseconds>(end_time - start_time).count() << " microseconds" << endl;
    cout << "Parallel Minimum: " << minVal << endl;
    cout << "Parallel Maximum: " << maxVal << endl;
    cout << "Parallel Sum: " << sum << endl;
    cout << "Parallel Average: " << average << endl;
    return (end_time - start_time).count();
}

int main() {
    vector<int> arr(10000000);
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = rand() % 10000;
    }
    
    float t1 = serialReduction(arr);
    cout << "---------------------------" << endl;
    float t2 = parallelReduction(arr);
    cout<<"SPEED UP FACTOR "<<t1/t2<<"\n";
    
    return 0;
}
