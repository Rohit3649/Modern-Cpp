#include <iostream>
using namespace std;

int partition(int arr[], int start, int end)
{
    int pEle = start;
    int left = start+1;
    int right = end;
    while(left <= right) {
        while((left <= right) && arr[left] <= arr[pEle]) {
            left++;
        }
        while((right >= left) && arr[right] > arr[pEle]) {
            right--;
        }
        if (left < right) {
            swap(arr[left], arr[right]);
            left++;
            right--;
        }
    }
    swap(arr[pEle], arr[right]);
    return right;
}

void quickSort(int arr[], int start, int end)
{
    if (start < end) {
        int pivot = partition(arr, start, end);
        quickSort(arr, start, pivot-1);
        quickSort(arr, pivot+1, end);
    }
}

int main()
{
    int arr[]{51,2,28,1,67,3};
    int size = sizeof(arr)/sizeof(arr[0]);;
    quickSort(arr, 0, size-1);
    for(int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}