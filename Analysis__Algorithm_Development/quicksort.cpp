#include "quicksort.h"
#include <iostream>
using namespace std;

template < class T>
void swap(T * a, T * b) {
	T temp = *a;
	*a = *b;
	*b = *a;
}

template<class T>
bool comp(const T &a, const T &b) {
	return a < b;
}

template < class T >
int partition(T arr[], const T first, const T last) {
	// Выбор опорного элемента по медиане трёх для разбиения Ломуто
	T mid = (first + last) / 2;
	if (comp(arr[mid], arr[first]))
		swap(arr[first], arr[mid]);
	else if (comp(arr[mid], arr[first]))
		swap(arr[first], arr[mid]);
	else if (comp(arr[mid], arr[first]))
		swap(arr[first], arr[mid]);
	int pivot = arr[last];

	int i = (first - 1);

	for (int j = first; j <= last - 1; j++) {
		if (comp(arr[j], pivot)) {
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[last]);
	return (i + 1);
}

template < class T>
void sort(T arr[], const T first, const T last) {
	if (comp(first, last)) {
		int pi = partition(arr, first, last);

		sort(arr, first, pi - 1);
		sort(arr, pi + 1, last);		
	}
}

template < class T>
void insertionSort(T arr[], const T size) {
	int i, key, j;
	for (i = 1; i < size; i++)
	{
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

template < class T >
void printArray(const T arr[], const T size) {
	int i;
	for (i = 0; i < size; i++)
		cout << arr[i] << " ";
	cout << endl;
}

template < class T >
void inputArray(T arr[], const T size) {
	int i;
	T a;
	for (i = 0; i < size; i++) {
		cin >> a;
		arr[i] = a;
	}
}

int main() {
	int N;
	cin >> N;
	
	int a[99999];
	inputArray(a, N);

	if (N <= 10) {
		insertionSort(a, N);
		cout << "Insertion Sort was invoked!" << endl;
	}
	else {
		sort(a, 0, N - 1);
		cout << "Quick Sort was invoked!" << endl;
	}
	printArray(a, N);
	
}