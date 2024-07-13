#include <iostream>
using namespace std;

void buildHeap(int arr[], int start, int end)
{
    int size = end;
    for(int i = size/2 - 1; i >= 0; i--) {
        int leftC = i*2 + 1;
        int rightC = i*2 + 2;
        int swapIndex = i;
        if (leftC <= size && arr[leftC] > arr[swapIndex]) {
            swapIndex = leftC;
        }
        if (rightC <= size && arr[rightC] > arr[swapIndex]) {
            swapIndex = rightC;
        }
        if (swapIndex != i) {
            swap(arr[i], arr[swapIndex]);
        }
    }

    for(int i = 0 ; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void hepifyUp(int arr[], int size, int index)
{
    for(int i = index; i >= 0; ) {
        int parent = (i%2==0) ? i/2-1 : i/2;
        if (arr[parent] < arr[i]) {
            swap(arr[i], arr[parent]);
            i = parent;
        }
        else {
            break;
        }
    }
}

void hepifyDown(int arr[], int size, int index=0)
{
    while(index < size) {
        int leftC = index*2+1;
        int rightC = index*2+2;
        int maxEle = index;
        if (leftC < size && arr[leftC] > arr[maxEle]) {
            maxEle = leftC;
        }
        if (rightC < size && arr[rightC] > arr[maxEle]) {
            maxEle = rightC;
        }
        if (maxEle != index) {
            swap(arr[index], arr[maxEle]);
            //hepifyDown(arr, size, maxEle);
            index = maxEle;
        }
        else {
            break;
        }
    }
}

void heapSort(int arr[], int size)
{
    buildHeap(arr, 0, size);
    for(int i = 0 ; i < size; i++) {
        swap(arr[0], arr[size-1-i]);
        hepifyDown(arr, size-i-1, 0);
    }
}

int main()
{
    int arr[]{5,2,28,1,67,3};
    int size = sizeof(arr)/sizeof(arr[0]);
    heapSort(arr, size);
    for(int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}