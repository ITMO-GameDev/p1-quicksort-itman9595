#include "pch.h"
#include "gtest/gtest.h"
#include <utility>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

template<typename K, typename V>
class Dictionary {
	using k_v = pair<K, V>;
private:
	int numItems;
	// ”зел внутри Dictionary
	struct Node {
		Node *lChild, *rChild; // лева€ и права€ дочерние элементы
		k_v item; // элемент
		bool isRed; // €вл€етс€ ли цвет красным

		Node(const k_v item) {
			this->item = item;
			this->lChild = this->rChild = NULL;
			this->isRed = true;
		}

		~Node() {
			if (lChild != NULL) { delete lChild; }
			if (rChild != NULL) { delete rChild; }
			lChild = rChild = NULL;
		}

		// вращает против часовой стрелки
		static Node* leftRotation(Node* nd) {
			printf("Left Rotation!!\n");
			Node* child = nd->rChild;
			Node* childLeft = child->lChild;

			child->lChild = nd;
			nd->rChild = childLeft;

			return child;
		}

		// вращает по часовой стрелке
		static Node* rightRotation(Node* nd) {
			printf("Right Rotation!!\n");
			Node* child = nd->lChild;
			Node* childRight = child->rChild;

			child->rChild = nd;
			nd->lChild = childRight;

			return child;
		}

		// проверка на красный цвет
		static int colorIsRed(Node* nd) {
			if (nd == NULL)
				return 0;
			return (nd->isRed == true);
		}

		// мен€ет цвета двух узлов
		static void swapColors(Node* nd1, Node* nd2) {
			bool temp = nd1->isRed;
			nd1->isRed = nd2->isRed;
			nd2->isRed = temp;
		}

		// вставл€ет узел в дерево
		static Node* put(const K& key, const V& value, Node* nd) {
			k_v item(key, value);
			if (nd == NULL)
				return new Node(item);

			if (item.second < nd->item.second)
				nd->lChild = put(key, value, nd->lChild);

			else if (item.second > nd->item.second)
				nd->rChild = put(key, value, nd->rChild);

			else
				return nd;

			if (colorIsRed(nd->rChild) && !colorIsRed(nd->lChild)) {
				nd = leftRotation(nd);
				swapColors(nd, nd->lChild);
			}

			if (colorIsRed(nd->lChild) && colorIsRed(nd->lChild->lChild)) {
				nd = rightRotation(nd);
				swapColors(nd, nd->rChild);
			}

			if (colorIsRed(nd->lChild) && colorIsRed(nd->rChild)) {
				nd->isRed = !nd->isRed;

				nd->lChild->isRed = false;
				nd->rChild->isRed = false;
			}

			return nd;
		}

		// пробегаетс€ по дочерним элементам по структуре "левый, корень, правый"
		static void inorder(Node* nd) {
			if (nd) {
				inorder(nd->lChild);
				cout << nd->item.first << " : " << nd->item.second << endl;
				inorder(nd->rChild);
			}
		}

		// получает значение через ключ
		static void getValueByKey(const K& key, V& value, Node* nd) {
			if (nd) {
				getValueByKey(key, value, nd->lChild);
				if (nd->item.first == key)
					value = nd->item.second;
				getValueByKey(key, value, nd->rChild);
			}
		}
	};
	Node *root;
	V valueFromKey;
	string data_types[13] = {
		"short",
		"unsigned short",
		"unsigned int",
		"int",
		"long",
		"unsigned long",
		"__int64",
		"unsigned __int64",
		"signed char",
		"unsigned char",
		"float",
		"double",
		"long double"
	};
public:
	// »тератор
	class Iterator {
		public:
			typedef k_v& reference;
			typedef k_v* pointer;
			typedef Dictionary& parent;
			Iterator(parent par) : par_(par) {
			};

			// Stack
			vector<Node*> map_vector;
			int index;
			void explore(Node* nd) {
				if (nd) {
					explore(nd->lChild);
					map_vector.push_back(nd);
					explore(nd->rChild);
				}
			}

			const K& key() const {
				Node* nd = map_vector[index];
				return nd->item.first;
			}

			const V& get() const {
				Node* nd = map_vector[index];
				return nd->item.second;
			}

			void set(const V& value) {
				Node* nd = map_vector[index];
				nd->item.second = value;
				map_vector[index] = nd;
			}

			bool hasNext() const {
				if (index < map_vector.size())
					return true;
				else
					return false;
			}

			void next() {
				index++;
			}

			bool hasPrev() const {
				if (index > -1)
					return true;
				else
					return false;
			}

			void prev() {
				index--;
			}
		private:
			parent par_;
	};
	
	Dictionary<K, V>::Iterator it;
	Dictionary() : it(*this) {
		root = NULL;
		numItems = 0;
		valueFromKey = NULL;
	}

	~Dictionary() {
		delete root;
		root = NULL;
		valueFromKey = NULL;
	}

	void put(const K& key, const V& value) {
		root = Node::put(key, value, root);
		root->isRed = false;
		numItems++;
	}

	void getAllValues() {
		Node::inorder(root);
	}

	const V& operator[](const K& key) const {
		valueFromKey = NULL;
		Node::getValueByKey(key, valueFromKey, root);
		if (valueFromKey == NULL) {
			string s1 = typeid(V).name(), s2 = "char const";
			V valueToPut = V("null");
			if (s1.find(s2) != string::npos) {
				return valueToPut;
			}
			else if (s1 == "short" || s1 == "unsigned short" || s1 == "unsigned int" || s1 == "int" ||
				s1 == "long" || s1 == "unsigned long" || s1 == "__int64" || s1 == "unsigned __int64") {
				valueToPut = V(0);
				return valueToPut;
			}
			else if (s1 == "signed char" || s1 == "unsigned char") {
				valueToPut = V('0');
				return valueToPut;
			}
			else if (s1 == "float" || s1 == "double" || s1 == "long double") {
				valueToPut = V(0.0);
				return valueToPut;
			}
			return valueToPut;
		}
		return valueFromKey;
	}

	V& operator[](const K& key) {
		valueFromKey = NULL;
		Node::getValueByKey(key, valueFromKey, root);
		if (valueFromKey == NULL) {
			string s1 = typeid(V).name(), s2 = "char const";
			V valueToPut = V("null");
			if (s1.find(s2) != string::npos) {
				root = Node::put(key, valueToPut, root);
				root->isRed = false;
				numItems++;
				return valueToPut;
			}
			else if (s1 == "short" || s1 == "unsigned short" || s1 == "unsigned int" || s1 == "int" ||
				s1 == "long" || s1 == "unsigned long" || s1 == "__int64" || s1 == "unsigned __int64") {
				valueToPut = V(0);
				root = Node::put(key, valueToPut, root);
				root->isRed = false;
				numItems++;
				return valueToPut;
			}
			else if (s1 == "signed char" || s1 == "unsigned char") {
				valueToPut = V('0');
				root = Node::put(key, valueToPut, root);
				root->isRed = false;
				numItems++;
				return valueToPut;
			}
			else if (s1 == "float" || s1 == "double" || s1 == "long double") {
				valueToPut = V(0.0);
				root = Node::put(key, valueToPut, root);
				root->isRed = false;
				numItems++;
				return valueToPut;
			}
			return valueToPut;
		}
		return valueFromKey;
	}

	bool contains(const K& key) {
		valueFromKey = NULL;
		Node::getValueByKey(key, valueFromKey, root);
		if (valueFromKey != NULL) {
			return true;
		}
		return false;
	}

	int size() const {
		return numItems;
	}

	// начало итератора
	Iterator iterator() {
		it.map_vector.clear();
		it.index = 0;
		it.explore(root);
		return it;
	}

	// конец итератора
	Iterator iterator_end() {
		it.map_vector.clear();
		it.explore(root);
		it.index = it.map_vector.size() - 1;
		return it;
	}
};

struct DictionaryTest : testing::Test {
	Dictionary<string, int> npc;
	DictionaryTest() {}
};

// ѕроверка на присутствие значени€ "pear" в Dictionary
TEST_F(DictionaryTest, checkIfValueIsPresent_1) {
	npc.put("pear", 10);
	EXPECT_EQ(true, npc.contains("pear"));
}

// ѕроверка на присутствие значени€ "apple1" в Dictionary
TEST_F(DictionaryTest, checkIfValueIsPresent_2) {
	npc.put("apple", 123);
	EXPECT_EQ(true, npc.contains("apple1"));
}

// ѕроверка на доступность значени€ через ключ "orange" в Dictionary
TEST_F(DictionaryTest, checkIfValueIsAccessibleViaOperator_1) {
	npc.put("orange", 9);
	npc.put("banana", 2);
	EXPECT_EQ(9, npc["orange"]);
}

// ѕроверка на доступность значени€ через ключ "orange1" в Dictionary, если значени€ нет, то он должен будет вернуть 0
TEST_F(DictionaryTest, checkIfValueIsAccessibleViaOperator_2) {
	npc.put("orange", 9);
	npc.put("banana", 2);
	EXPECT_EQ(9, npc["orange1"]);
}

// ѕроверка на доступность значени€ через ключ "orange1" в Dictionary, если значени€ нет, то он должен будет вернуть 0
TEST_F(DictionaryTest, checkIfValueIsAccessibleViaOperator_3) {
	npc.put("orange", 9);
	npc.put("banana", 2);
	EXPECT_EQ(0, npc["orange1"]);
}

// ѕроверка на доступность значени€ в следующем индексе итератора Dictionary
TEST_F(DictionaryTest, checkIfIteratorHasNextValue_1) {
	auto it = npc.iterator();
	EXPECT_EQ(true, it.hasNext());
}

// ѕроверка на доступность значени€ в следующем индексе итератора Dictionary
TEST_F(DictionaryTest, checkIfIteratorHasNextValue_2) {
	npc.put("apple", 10);
	auto it = npc.iterator();
	EXPECT_EQ(true, it.hasNext());
}

// ѕроверка на доступность значени€ в предыдущем индексе итератора Dictionary, и давайте присвоим индекс на 0
TEST_F(DictionaryTest, checkIfIteratorHasPrevValue_1) {
	npc.put("apple", 10);
	npc.put("orange", 20);
	npc.put("banana", 30);
	auto it = npc.iterator_end();
	it.index = -1;
	EXPECT_EQ(true, it.hasPrev());
}

// ѕроверка на доступность значени€ в предыдущем индексе итератора Dictionary, индекс будет равен на (размер значении в итераторе - 1)
TEST_F(DictionaryTest, checkIfIteratorHasPrevValue_2) {
	npc.put("apple", 10);
	npc.put("orange", 20);
	npc.put("banana", 30);
	auto it = npc.iterator_end();
	EXPECT_EQ(true, it.hasPrev());
}

int main(int argc, char *argv[]) {
	/*if (npc.contains("pear")) {
		cout << npc["pear"] << endl;
	}

	if (npc.contains("orange1")) {
		cout << npc["orange"] << endl;
	}
	else {
		cout << "doesn't exist" << endl;
	}

	cout << endl << "|--------- KEY : VALUE ---------|" << endl << endl;
	npc.getAllValues();

	cout << endl << "|--------- ITERATOR KEY : VALUE ---------|" << endl << endl;
	for (auto it = npc.iterator(); it.hasNext(); it.next())
		cout << it.key() << " : " << it.get() << endl;

	cout << endl << "|--------- ITERATOR KEY : VALUE ---------|" << endl << endl;
	for (auto it = npc.iterator_end(); it.hasPrev(); it.prev())
		cout << it.key() << " : " << it.get() << endl;*/

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}