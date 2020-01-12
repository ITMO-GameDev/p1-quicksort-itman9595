#include "pch.h"
#include <iostream>
#include "gtest/gtest.h"
#include <functional>
#include <limits>
#include <stdlib.h>
#include <math.h>

using namespace std;
using namespace std::placeholders;
#define N 8

template <class T>
class Array {
public:
	T *arr;
	int capacity;
	int nextIndex;

	class Iterator {
	public:
		typedef T& reference;
		typedef T* pointer;
		typedef Array& parent;
		Iterator(parent par) : par_(par) { itNextElement = 0; };
		parent par_;
	private:
		int itNextElement;
	public:

		T get() const {
			return *(par_.arr + itNextElement);
		}

		void set(const T& value) {
			par_[itNextElement] = value;
		}

		void insert(const T& value) {
			par_.insert(itNextElement, value);
		}

		void remove() {
			par_.remove(itNextElement);
		}

		void next() {
			itNextElement++;
		}

		void prev() {
			itNextElement--;
		}

		void toIndex(int index) {
			itNextElement = index;
		}

		bool hasNext() const {
			if (itNextElement < par_.size())
				return true;
			else
				return false;
		}

		bool hasPrev() const {
			if (itNextElement > -1)
				return true;
			else
				return false;
		}
	};

	Array<T>::Iterator it; // это референс к нашему итератору

	Array() : it(*this) { // здесь мы передаём наш родитель в итератор, чтобы обмениваться данными между классами
		arr = new T[N];
		for (int i = 0; i < N; i++)
			arr[i] = 0;
		capacity = N;
		nextIndex = 0;
	}

	Array(int capacity) : it(*this) {
		this->capacity = capacity;
		arr = new T[capacity];
		for (int i = 0; i < capacity; i++)
			arr[i] = 0;
		nextIndex = 0;
	}

	~Array() {
		delete [] arr;
	}

	void resize(int index = 1) {
		int newCapacity = capacity + index;
		T* newArr = new T[newCapacity];
		
		memcpy(newArr, arr, capacity * sizeof(T));
		
		delete [] arr;
		arr = newArr;

		if(index != 1)
			for (int i = capacity; i < newCapacity; i++)
				arr[i] = 0;

		capacity = newCapacity;
	}
	
	void insert(const T& value) {
		if (nextIndex == capacity)
			resize();
		arr[nextIndex++] = value;
	}

	void insert(int index, const T& value) {
		if(index > capacity)
			resize(index - capacity);
		else
			resize();

		nextIndex = index - 1;
		for (int i = capacity - 2; i >= nextIndex; i--)
			arr[i + 1] = arr[i];
		arr[nextIndex] = value;
	}

	void remove(int index) {
		for (int i = index - 1; i < capacity - 1; i++)
			arr[i] = arr[i + 1];
		arr[capacity - 1] = 0;
		nextIndex--;
	}

	T &operator[] (int index) {
		return *(arr + index);
	}

	T operator[] (int index) const {
		return *(arr + index);
	}

	int size() const {
		return capacity;
	}
	
	Iterator iterator() {
		return it;
	}

	Iterator reversed_iterator() {
		it.toIndex(it.par_.size() - 1);
		return it;
	}
};

struct ContainersTest : testing::Test {
	Array<int> a = Array<int>(10);
	ContainersTest() {
	}
};

// Проверка на доступность значения через индекс 3 в массиве
TEST_F(ContainersTest, checkIfValueIsAccessibleViaOperator_1) {	
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	EXPECT_EQ(3, a[3]);
}

// Проверка на доступность значения через индекс 2 в массиве
TEST_F(ContainersTest, checkIfValueIsAccessibleViaOperator_2) {
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	EXPECT_EQ(0, a[2]);
}

// Проверка на доступность значения в следующем индексе итератора массива
TEST_F(ContainersTest, checkIfIteratorHasNextValue_1) {
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	auto it = a.iterator();
	EXPECT_EQ(true, it.hasNext());
}

// Проверка на доступность значения в следующем индексе итератора массива, и в случае, если индекс будет равен размеру массива
TEST_F(ContainersTest, checkIfIteratorHasNextValue_2) {
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	auto it = a.iterator();
	it.toIndex(a.size());
	EXPECT_EQ(true, it.hasNext());
}

// Проверка на доступность значения в предыдущем индексе итератора массива
TEST_F(ContainersTest, checkIfIteratorHasPrevValue_1) {
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	auto it = a.reversed_iterator();
	EXPECT_EQ(true, it.hasPrev());
}

// Проверка на доступность значения в предыдущем индексе итератора массива, и в случае, если индекс будет равен "-1"
TEST_F(ContainersTest, checkIfIteratorHasPrevValue_2) {
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	auto it = a.reversed_iterator();
	it.toIndex(-1);
	EXPECT_EQ(true, it.hasPrev());
}

// Проверка на равенство значения в субиндексе массива, здесь массив в индексе 3 должен быть равен 3, но после устранения значения на позиции 3, массив начнёт сдвигаться
TEST_F(ContainersTest, checkForArrayChange) {
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	a.remove(3);
	a.remove(6);
	EXPECT_EQ(4, a[3]);
}

int main(int argc, char *argv[]) {
	Array<int> a = Array<int>(10);
	for (int i = 0; i < a.size(); i++)
		a.insert(i);
	
	for (int i = 0; i < a.size(); i++)
		a[i] *= 2;

	for (auto i = a.iterator(); i.hasNext(); i.next())
		cout << i.get() << endl;

	cout << endl;
	int count = 0;
	for (auto i = a.iterator(); i.hasNext(); i.next()) {
		if (count == 3 || count == 6) { // на 3 и 6 позиции вставляет число 9 в массив
			i.insert(9);
		} else if (count == 5 || count == 7) { // на 5 и 7 позиции убирает число
			i.remove();
		}
		count++;
	}

	for (auto i = a.iterator(); i.hasNext(); i.next())
		cout << i.get() << endl;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}