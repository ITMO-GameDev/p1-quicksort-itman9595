#include "pch.h"
#include "gtest/gtest.h"
#include <utility>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

template<typename K, typename V>
class Dictionary {
	using k_v = pair<K, V>;
	public:
		/*class Iterator {
			public:
				typedef k_v& reference;
				typedef k_v* pointer;
				typedef Dictionary& parent;
				Iterator(parent par) : par_(par) {};

				const K& key() const {
					return par_.root
				}

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
					if (itNextElement > 1)
						return true;
					else
						return false;
				}
			private:
				parent par_;
				int itNextElement;
		};*/
		Dictionary() {
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

		/*Iterator iterator() {
			return it;
		}*/

	private:
		int numItems;
		struct Node {
			Node *lChild, *rChild;
			k_v item;
			bool isRed;

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

			static Node* leftRotation(Node* nd) {
				printf("Left Rotation!!\n");
				Node* child = nd->rChild;
				Node* childLeft = child->lChild;

				child->lChild = nd;
				nd->rChild = childLeft;

				return child;
			}

			static Node* rightRotation(Node* nd) {
				printf("Right Rotation!!\n");
				Node* child = nd->lChild;
				Node* childRight = child->rChild;

				child->rChild = nd;
				nd->lChild = childRight;

				return child;
			}

			static int colorIsRed(Node* nd) {
				if (nd == NULL)
					return 0;
				return (nd->isRed == true);
			}

			static void swapColors(Node* nd1, Node* nd2) {
				bool temp = nd1->isRed;
				nd1->isRed = nd2->isRed;
				nd2->isRed = temp;
			}

			static Node* put(const K& key, const V& value, Node* nd) {
				k_v item (key, value);
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

			static void inorder(Node* nd) {
				if (nd) {
					inorder(nd->lChild);
					cout << nd->item.first << " : " << nd->item.second << endl;
					inorder(nd->rChild);
				}
			}

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
};

int main(int argc, char *argv[]) {

	Dictionary<string, int> npc;
	npc.put("orange", 10);
	npc.put("apple", 20);
	npc.put("pear", 30);
	npc.put("pineapple", 40);
	npc.put("banana", 50);
	npc.put("ananas", 25);

	if (npc.contains("pear")) {
		cout << npc["pear"] << endl;
	}

	if (npc.contains("orange1")) {
		cout << npc["orange"] << endl;
	}
	else {
		cout << "doesn't exist" << endl;
	}

	cout << endl << "|--------- KEY : VALUE ---------|" << endl << endl;
	cout << "orange :" << npc["orange"] << endl;
	cout << "apple :" << npc["apple"] << endl;
	cout << "pear :" << npc["pear"] << endl;
	cout << "pineapple :" << npc["pineapple"] << endl;
	cout << "banana :" << npc["banana"] << endl;
	cout << "ananas :" << npc["ananas"] << endl;
	
	/*for (auto it = npc.iterator(); it.hasNext();it.next()) {
		cout << it.key() << " " << it.get() << endl;
	}*/
	
	//testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
}