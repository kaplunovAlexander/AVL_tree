#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <stack>
using namespace std;

static string ch_udia_hor = "'->", ch_ddia_hor = ",->", ch_ver_spa = "| ";

struct BST {
	int height;
	int value;
	BST* left;
	BST* right;
	BST() : value(0), left(nullptr), right(nullptr) {}
	BST(int x) : value(x), left(nullptr), right(nullptr) {}
	BST(int x,  BST* left,  BST* right) : value(x), left(left), right(right) {}
};

int findDepth(BST* node, int const& d = 0) {
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

void setNodeDepth(BST*& node, bool const& flag = true) {
	if (node) {
		node->height = findDepth(node);
		if (flag) {
			if (node->right) {
				setNodeDepth(node->right);
			}
			if (node->left) {
				setNodeDepth(node->left);
			}
		}
	}
	else return;
}

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

void printTree(BST const* node, string const& rpref = "", string const& cpref = "", string const& lpref = "") {
	if (!node) return;
	if (node->right)
		printTree(node->right, rpref + "  ", rpref + ch_ddia_hor, rpref + ch_ver_spa);
	cout << cpref << node->value << '\n';
	if (node->left)
		printTree(node->left, lpref + ch_ver_spa, lpref + ch_udia_hor, lpref + "  ");
}

BST* findMaxLeft(BST* node, stack <BST*>& branch) {
	if (!node->right) return node;
	branch.push(node);
	findMaxLeft(node->right, branch);
}

BST* findMinRight(BST* node, stack <BST*>& branch) {
	if (!node->left) return node;
	branch.push(node);
	findMinRight(node->left, branch);
}


BST* findNode(BST* node, int data, stack <BST*>& branch) {
	if (!node) return 0;
	if (node->value == data) {
		return node;
	}
	if (node->value > data) {
		branch.push(node);
		return findNode(node->left, data, branch);
	}
	if (node->value < data) {
		branch.push(node);
		return findNode(node->right, data, branch);
	}
}

void deleteNode(BST* node, BST* prev) {
	if (node->right && node->left) {
		stack <BST*> branch;
		BST* new_node = findMaxLeft(node->left, branch);
		BST* prev_new_node = branch.top();
		prev_new_node->right = new_node->left;
		if (prev) {
			if (prev->right == node) prev->right = new_node;
			else prev->left = new_node;
		}
		new_node->right = node->right;
		if (node->left != new_node) new_node->left = node->left;
		delete node;
		setNodeDepth(prev_new_node->right);
		setNodeDepth(new_node, false);
		for (; !branch.empty();branch.pop()) {
			try {
				setNodeDepth(branch.top(), false);
			}
			catch (...) {
				break;
			}
		}
		return;
	}
	else if (node->right) {
		BST* new_node = node->right;
		if (prev) {
			if (prev->right == node) prev->right = new_node;
			else prev->left = new_node;
		}
		delete node;
		setNodeDepth(new_node, false);
		return;
	}
	else if (node->left) {
		BST* new_node = node->left;
		if (prev) {
			if (prev->right == node) prev->right = new_node;
			else prev->left = new_node;
		}
		delete node;
		setNodeDepth(new_node, false);
		return;
	}
	if (prev) {
		if (prev->right == node) prev->right = nullptr;
		else prev->left = nullptr;
	}
	delete node; 
}

int getHeight(BST* node) {
	return node == nullptr ? -1 : node->height;
}

int getBalance(BST* node) {
	return node == nullptr ? 0 : (getHeight(node->right) - getHeight(node->left));
}

void Swap(BST*& a, BST*& b) {
	swap(a->value, b->value);
	swap(a->height, b->height);
}

void rightRotate(BST*& node) {
	Swap(node, node->left);
	BST* node_right = node->right;
	node->right = node->left;
	node->left = node->left->left;
	node->right->left = node->right->right;
	node->right->right = node_right;
	setNodeDepth(node, false);
	setNodeDepth(node->right, false);
}

void leftRotate(BST*& node) {
	Swap(node, node->right);
	BST* node_left = node->left;
	node->left = node->right;
	node->right = node->right->right;
	node->left->right = node->left->left;
	node->left->left = node_left;
	setNodeDepth(node, false);
	setNodeDepth(node->left, false);
}

void balance(BST*& node) {
	while (getBalance(node) < -1) {
		while (getBalance(node->left) >= 1) leftRotate(node->left);
		rightRotate(node); 
	}
	while (getBalance(node) > 1) {
		while (getBalance(node->right) <= -1) rightRotate(node->right);
		leftRotate(node);
	}
}

void balanceTree(BST*& node) {
	if (node) balance(node);
	if (node->right) balanceTree(node->right);
	if (node->left) balanceTree(node->left);
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
	setNodeDepth(tree);
	balanceTree(tree);
	setNodeDepth(tree);
	// вывод бинарного дерева
	printTree(tree);
	// вставка элемента
	int new_node1;
	cin >> new_node1;
	addNode(tree, new_node1);
	setNodeDepth(tree);
	balanceTree(tree);
	setNodeDepth(tree, false);
	printTree(tree);
	// удаление элемента
	int data;
	stack <BST*> branch;
	cin >> data;
	BST* new_node = findNode(tree, data, branch);
	if (new_node){
		deleteNode(new_node, branch.top());
		for (; !branch.empty();) {
			setNodeDepth(branch.top(), false);
			branch.pop();
		}
		balanceTree(tree);
		setNodeDepth(tree, false);
		printTree(tree);
	}
	else cout << "Node not found\n";
	return 0;
}