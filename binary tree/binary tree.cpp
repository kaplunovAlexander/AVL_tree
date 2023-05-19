#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
using namespace std;

static string ch_udia_hor = "'->", ch_ddia_hor = ".->", ch_ver_spa = "| ";

struct BST {
	int key;
	int value;
	BST* left;
	BST* right;
	BST() : value(0), left(nullptr), right(nullptr) {}
	BST(int x) : value(x), left(nullptr), right(nullptr) {}
	BST(int x,  BST* left,  BST* right) : value(x), left(left), right(right) {}
};

void addNode(BST*& node, int val) {
	if (!node) {
		node = new BST(val);
		return;
	}
	else if (node->value == val) return;
	else if (node->value > val) addNode(node->left, val);
	else if (node->value < val) addNode(node->right, val);
}

BST* createTree( BST* node, vector <int> &vec) {
	for (int val : vec) {
		addNode(node, val);
	}
	return node;
}

int findDepth(BST* node, int d) {
	if (node) {
		int depth_l = d, depth_r = d;
		if (node->right) {
			depth_r++;
			depth_r = findDepth(node->right, depth_r);
		}
		if (node->left) {
			depth_l++;
			depth_l = findDepth(node->left, depth_l);
		}
		return max(depth_l, depth_r);
	}
	else return -1;
}

void setNodeDepth(BST*& node, int d) {
	if (node) {
		int depth_l = d, depth_r = d;
		node->key = d;
		if (node->right) {
			depth_r--;
			setNodeDepth(node->right, depth_r);
		}
		if (node->left) {
			depth_l--;
			setNodeDepth(node->left, depth_l);
		}
		return;
	}
	else return;
}

void printTree(BST const* node, string const& rpref = "", string const& cpref = "", string const& lpref = "") {
	if (!node) return;
	if (node->right)
		printTree(node->right, rpref + "  ", rpref + ch_ddia_hor, rpref + ch_ver_spa);
	cout << cpref << to_string(node->value) << '\n';
	if (node->left)
		printTree(node->left, lpref + ch_ver_spa, lpref + ch_udia_hor, lpref + "  ");
}



int main() {
	setlocale(LC_ALL, "");
	// создание вектора чисел из потока ввода
	string str_numbers, curr_number = "";
	vector <int> vec_numbers;
	getline(cin, str_numbers);
	for (char c : str_numbers) {
		if (isdigit(c)) {
			curr_number += c;
		}
		else if (!curr_number.empty()) {
			vec_numbers.push_back(stoi(curr_number));
			curr_number.clear();
		}
	}
	if (!curr_number.empty()) {
		vec_numbers.push_back(stoi(curr_number));
		curr_number.clear();
	}
	// создание бинарного дерева
	BST* tree = nullptr;
	tree = createTree(tree, vec_numbers);
	int depth = findDepth(tree, 0);
	setNodeDepth(tree, depth);
	// вывод бинарного дерева
	printTree(tree);
	// вставка элемента
	int new_node;
	cin >> new_node;
	addNode(tree, new_node);
	return 0;
}