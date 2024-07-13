#include <iostream>
using namespace std;

void merge(int arr[], int start, int mid, int end)
{
    int leftStart = start;
    int rightStart = mid;
    int tempArr[100];
    int tempIndex = 0;
    while((leftStart < mid) && rightStart <= end) {
        if (arr[leftStart] <= arr[rightStart]) {
            tempArr[tempIndex++] = arr[leftStart];
            leftStart++;
        }
        else {
            tempArr[tempIndex++] = arr[rightStart];
            rightStart++;
        }
    }

    while(leftStart < mid) {
        tempArr[tempIndex++] = arr[leftStart];
        leftStart++;
    }

    while(rightStart <= end) {
        tempArr[tempIndex++] = arr[rightStart];
        rightStart++;
    }
    tempIndex = 0;
    for(int i = start; i <= end; i++) {
        arr[i] = tempArr[tempIndex++];
    }
}

void mergeSort(int arr[], int start, int end)
{
    if (start < end) {
        int mid = (start+end)/2;
        mergeSort(arr, start, mid);
        mergeSort(arr, mid+1, end);
        merge(arr, start, mid+1, end);
    }
}

int main()
{
    int arr[]{51,2,28,1,67,3};
    int size = sizeof(arr)/sizeof(arr[0]);;
    mergeSort(arr, 0, size-1);
    for(int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}