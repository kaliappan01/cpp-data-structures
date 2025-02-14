#include <bits/stdc++.h>

class BinaryHeap
{
private:
    int size;
    std::vector<int> arr;
    void heapify(int i);

public:
    bool insert(int val);
    BinaryHeap() = default;
    int parent(int v);
    int leftChild(int v);
    int rightChild(int v);
    int getMin();
    int removeMin();
    void printHeap();
};

int BinaryHeap::leftChild(int i)
{
    return 2 * i + 1;
}
int BinaryHeap::rightChild(int i)
{
    return 2 * i + 2;
}
int BinaryHeap::parent(int i)
{
    return (i - 1) / 2;
}

void swap(int &a, int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
void BinaryHeap::heapify(int i)
{
    int smallest = i;
    int left = leftChild(i);
    int right = rightChild(i);
    if (BinaryHeap::arr[smallest] > arr[left])
    {
        smallest = left;
    }
    if (BinaryHeap::arr[smallest] > arr[right])
    {
        smallest = right;
    }
    if (smallest != i)
    {
        swap(arr[i], arr[smallest]);
        heapify(smallest);
    }
}

int BinaryHeap::getMin()
{
    if (size == 0)
    {
        std::cout << "Heap underflow";
    }
    return arr[0];
}

int BinaryHeap::removeMin()
{
    if (size == 0)
    {
        std::cout << "Heap underflow";
    }
    int minV;
    if (size == 1)
    {
        minV = arr[0];
        arr.pop_back();
        size--;
        return minV;
    }
    minV = arr[0];
    arr[0] = arr.back();
    arr.pop_back();
    size--;
    heapify(0);
    return minV;
}

bool BinaryHeap::insert(int val)
{
    arr.push_back(val);
    size = arr.size();
    int idx = size - 1;
    while (idx > 0 && arr[parent(idx)] > arr[idx])
    {
        swap(arr[parent(idx)], arr[idx]);
        idx = parent(idx);
    }
    return true;
}
void BinaryHeap::printHeap()
{
    for (int i : arr)
    {
        std::cout << i << " ";
    }
}

int main()
{
    BinaryHeap binHeap;
    binHeap.insert(1);
    binHeap.insert(5);
    binHeap.insert(0);
    std::cout << "min val " << binHeap.getMin() << std::endl;
    binHeap.insert(-5);
    std::cout << "min val " << binHeap.getMin() << std::endl;
    return 0;
}