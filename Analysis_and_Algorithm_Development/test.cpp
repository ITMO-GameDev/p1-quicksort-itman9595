#include "pch.h"
#include "gtest/gtest.h"
#include <functional>
#include <limits>
#include <stdlib.h>

using namespace std;

#define size 99999

auto glambda = [](int a, int b) { return a < b; };

class QuicksortUnitTests {
public:

	template < class T>
	void swap(T *a, T *b) {
		T temp = *a;
		*a = *b;
		*b = temp;
	}

	template <class T>
	bool comp(const T *a, const T *b) {
		return a < b;
	}

	template < class T, class Compare >
	int partition(T *first, T *last, Compare comp) {
		// ¬ыбор опорного элемента по медиане трЄх дл€ разбиени€ Ћомуто

		int low = 0;
		int high = distance(first, last);

		int mid = (low + high) / 2;
		if (comp(first[mid], first[low]))
			swap(&first[low], &first[mid]);
		if (comp(first[high], first[low]))
			swap(&first[low], &first[high]);
		if (comp(first[mid], first[high]))
			swap(&first[high], &first[mid]);
		int pivot = first[high]; // выбор опорной точки

		int i = low - 1;
		for (int j = low; j <= high; j++) {
			if (comp(first[j], pivot)) {
				i++;
				swap(&first[i], &first[j]);
			}
		}
		swap(&first[i + 1], &first[high]);
		return (i + 1);
	}

	template < typename T, typename Compare >
	void sort(T *first, T *last, Compare comp) {
		int low = 0;
		int high = distance(first, last);
		if (comp(low, high)) {
			int pi = partition(first, last, comp);
			sort(first, first + pi - 1, comp);
			sort(first + pi + 1, last, comp);
		}
	}

	template < class T>
	void insertionSort(T *first, T *last) {
		int i, key, j;
		int N = distance(first, last);
		for (i = 1; i < N; i++)
		{
			key = first[i];
			j = i - 1;

			while (j >= 0 && first[j] > key)
			{
				first[j + 1] = first[j];
				j = j - 1;
			}
			first[j + 1] = key;
		}
	}

	// ѕровер€ет на то, пустой ли массив
	template < class T >
	string empty() {
		size_t N = 0;
		T list[99999] = {};
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует одно значение
	template < class T >
	string oneValue() {
		size_t N = 1;
		T list[99999] = { 1 };
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует два значени€ изначально в правильном пор€дке
	template < class T >
	string twoValues_correctOrder() {
		size_t N = 2;
		T list[99999] = { 1, 2 };
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует два значени€ изначально в неправильном пор€дке
	template < class T >
	string twoValues_wrongOrder() {
		size_t N = 2;
		T list[99999] = { 2, 1 };
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует три значени€ изначально в правильном пор€дке
	template < class T >
	string threeValues_correctOrder() {
		size_t N = 3;
		T list[99999] = { 1, 2, 3 };
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует три значени€ изначально в неправильном пор€дке
	template < class T >
	string threeValues_wrongOrder() {
		size_t N = 3;
		T list[99999] = { 3, 1, 2 };
		insertionSort(list, list + N);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует похожие значени€
	template < class T >
	string equalValues() {
		size_t N = 6;
		T list[99999] = { 1, 1, 1, 1, 1, 1 };
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует значени€ в обратном пор€дке сортировкой вставками
	template < class T >
	string reversedOrder() {
		size_t N = 10;
		T list[99999] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
		insertionSort(list, list + N);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

	// —ортирует зараннее заданные значени€
	template < class T >
	string predefinedValues() {
		size_t N = 11;
		T list[99999] = { 9, 2, 3, 1, 6, 1, 5, 7, 2, 5, 10 };
		sort(list, list + N - 1, glambda);
		string result = "";
		for (int i = 0; i < N; i++)
			result = result + " " + to_string(list[i]);
		return result;
	}

};

struct QuicksortTest : testing::Test
{
	QuicksortUnitTests *qq;
	QuicksortTest() {
		qq = new QuicksortUnitTests;
	}

	~QuicksortTest() {
		delete qq;
	}
};

// ѕровер€ет на то, пустой ли массив
TEST_F(QuicksortTest, EmptyArray) {
	EXPECT_EQ("", qq->empty<int>());
}

// —ортирует одно значение
TEST_F(QuicksortTest, OneValue) {
	EXPECT_EQ(" 1", qq->oneValue<int>());
}

// —ортирует два значени€ изначально в правильном пор€дке
TEST_F(QuicksortTest, TwoValuesCorrectOrder) {
	EXPECT_EQ(" 1 2", qq->twoValues_correctOrder<int>());
}

// —ортирует два значени€ изначально в неправильном пор€дке
TEST_F(QuicksortTest, TwoValuesWrongOrder) {
	EXPECT_EQ(" 1 2", qq->twoValues_wrongOrder<int>());
}

// —ортирует три значени€ изначально в правильном пор€дке
TEST_F(QuicksortTest, ThreeValuesCorrectOrder) {
	EXPECT_EQ(" 1 2 3", qq->threeValues_correctOrder<int>());
}

// —ортирует три значени€ изначально в неправильном пор€дке сортировкой вставками
TEST_F(QuicksortTest, ThreeValuesWrongOrder) {
	EXPECT_EQ(" 1 2 3", qq->threeValues_wrongOrder<int>());
}

// —ортирует похожие значени€
TEST_F(QuicksortTest, EqualValues) {
	EXPECT_EQ(" 1 1 1 1 1 1", qq->equalValues<int>());
}

// —ортирует значени€ в обратном пор€дке сортировкой вставками
TEST_F(QuicksortTest, ReversedOrder) {
	EXPECT_EQ(" 1 2 3 4 5 6 7 8 9 10", qq->reversedOrder<int>());
}

// —ортирует зараннее заданные значени€
TEST_F(QuicksortTest, PredefinedValues) {
	EXPECT_EQ(" 1 1 2 2 3 5 5 6 7 9 10", qq->predefinedValues<int>());
}

int main(int argc, char *argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}