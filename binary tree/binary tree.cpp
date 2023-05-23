#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <stack>
#include <chrono>
#include <fstream>
using namespace std;

static string ch_udia_hor = "'->", ch_ddia_hor = ",->", ch_ver_spa = "| ", sep = "--------------------------------------------------------------------\n";
ofstream tree_f("tree.txt", ios::app);
ofstream tree_ans("output ans.txt", ios::app);

int menu(int k) {
	if (!k)
		cout << "\n\t1. Создать АВЛ-дерево\n\t2. Вывести АВЛ-дерево на экран\n\t3. Найти элемент АВЛ-дерева\n\t4. Добавить элемент в АВЛ-дерево\n\t5. Удалить элемент АВЛ-дерева\n\t6. Завершить работу программы\n>>";
	else cout << "7. Открыть меню\n>>";
	cin >> k;
	if (k >= 1 && k <= 7) return k;
	else return -1;
}

struct BST {
	int height;
	int value;
	BST* left;
	BST* right;
	BST() : value(0), left(nullptr), right(nullptr) {}
	BST(int x) : value(x), left(nullptr), right(nullptr) {}
	BST(int x, BST* left, BST* right) : value(x), left(left), right(right) {}
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

BST* createTree(BST* node, vector <int>& vec) {
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

void printTreeFile(BST const* node, string const& rpref = "", string const& cpref = "", string const& lpref = "") {
	if (!node) return;
	if (node->right)
		printTreeFile(node->right, rpref + "  ", rpref + ch_ddia_hor, rpref + ch_ver_spa);
	tree_f << cpref << node->value << '\n';
	if (node->left)
		printTreeFile(node->left, lpref + ch_ver_spa, lpref + ch_udia_hor, lpref + "  ");
}

void printTreeAns(BST const* node, string const& rpref = "", string const& cpref = "", string const& lpref = "") {
	if (!node) return;
	if (node->right)
		printTreeAns(node->right, rpref + "  ", rpref + ch_ddia_hor, rpref + ch_ver_spa);
	tree_ans << cpref << node->value << '\n';
	if (node->left)
		printTreeAns(node->left, lpref + ch_ver_spa, lpref + ch_udia_hor, lpref + "  ");
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

BST* deleteNode(BST*& node, BST* prev) {
	if (node->right && node->left) {
		stack <BST*> branch;
		BST* new_node = findMaxLeft(node->left, branch);
		BST* prev_new_node = branch.empty() ? nullptr : branch.top();
		if (prev_new_node) prev_new_node->right = new_node->left;
		if (prev) {
			if (prev->right == node) prev->right = new_node;
			else prev->left = new_node;
		}
		new_node->right = node->right;
		if (node->left != new_node) new_node->left = node->left;
		delete node;
		if (prev_new_node) if (prev_new_node->right) setNodeDepth(prev_new_node->right);
		setNodeDepth(new_node, false);
		for (; !branch.empty(); branch.pop()) {
			try {
				setNodeDepth(branch.top(), false);
			}
			catch (...) {
				break;
			}
		}
		return node = new_node;
	}
	else if (node->right) {
		BST* new_node = node->right;
		if (prev) {
			if (prev->right == node) prev->right = new_node;
			else prev->left = new_node;
		}
		delete node;
		setNodeDepth(new_node, false);
		return node = new_node;
	}
	else if (node->left) {
		BST* new_node = node->left;
		if (prev) {
			if (prev->right == node) prev->right = new_node;
			else prev->left = new_node;
		}
		delete node;
		setNodeDepth(new_node, false);
		return node = new_node;
	}
	if (prev) {
		if (prev->right == node) prev->right = nullptr;
		else prev->left = nullptr;
	}
	delete node;
	return node = nullptr;
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
	else return;
	if (node->right) balanceTree(node->right);
	if (node->left) balanceTree(node->left);
}

int main() {
	setlocale(LC_ALL, "");
	int key = 0;
	BST* tree = nullptr;
	key = menu(key);
	while (true) {
		switch (key) {
		case -1:
			if (key == -1) {
				cout << "Invalid input\n";
				cin.clear();
				cin.ignore();
			}
			break;
		case 1: {
			tree = nullptr;
			cout << "1. Создать АВЛ-дерево из потока ввода\n2. Создать АВЛ-дерево, заполненное случайными числами(-99 до 99)\n>>";
			int flag;
			string str_numbers, curr_number = "";
			vector <int> vec_numbers;
			cin >> flag;
			switch (flag) {
			case 1: {
				cout << "Введите числа, из которых будет состоять дерево\n>>";
				cin.clear();
				cin.ignore();
				getline(cin, str_numbers);
				auto start_time = chrono::high_resolution_clock::now();
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
				tree = createTree(tree, vec_numbers);
				setNodeDepth(tree);
				auto start_bal = chrono::high_resolution_clock::now();
				balanceTree(tree);
				auto end_bal = chrono::high_resolution_clock::now();
				auto total_bal = chrono::duration_cast<chrono::microseconds>(end_bal - start_bal).count();
				setNodeDepth(tree);
				auto end_time = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
				if (tree) cout << "Дерево создано\n";
				else cout << "the tree is empty\n";
				cout << "Время балансировки: " << total_bal << " мкс" << '\n';
				cout << "Время создания дерева: " << duration << " мкс" << endl;
				printTreeFile(tree);
				printTreeAns(tree);
				tree_f << sep;
				tree_ans << sep;
				break;
			}
			case 2: {
				tree = nullptr;
				int N;
				cout << "Введите количество элементов дерева\n>>";
				cin >> N;
				auto start_time = chrono::high_resolution_clock::now();
				for (int i = 0; i < N; i++) {
					vec_numbers.push_back((rand() % 199) - 100);
				}
				tree = createTree(tree, vec_numbers);
				setNodeDepth(tree);
				auto start_bal = chrono::high_resolution_clock::now();
				balanceTree(tree);
				auto end_bal = chrono::high_resolution_clock::now();
				auto total_bal = chrono::duration_cast<chrono::microseconds>(end_bal - start_bal).count();
				setNodeDepth(tree);
				auto end_time = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
				if (tree) cout << "Дерево создано\n";
				else cout << "the tree is empty\n";
				cout << "Время балансировки: " << total_bal << " мкс" << '\n';
				cout << "Время создания дерева: " << duration << " мкс" << endl;
				printTreeFile(tree);
				printTreeAns(tree);
				tree_f << sep;
				tree_ans << sep;
				break;
			}
			default: {
				cout << "Invalid input\n";
				break;
			}
			}
			break;
		}
		case 2: {
			// вывод бинарного дерева
			printTree(tree);
			break;
		}
		case 3: {
			int new_node2;
			stack <BST*> branch;
			cout << "Введите элемент для поиска\n>>";
			cin >> new_node2;
			cin.sync();
			auto start_time = chrono::high_resolution_clock::now();
			if (findNode(tree, new_node2, branch)) cout << "Элемент найден\n";
			else cout << "Элемент не найден\n";
			auto end_time = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
			cout << "Время поиска элемента: " << duration << " мкс" << endl;
			break;
		}
		case 4: {
			// вставка элемента
			int new_node1;
			cout << "Введите элемент для вставки\n>>";
			cin >> new_node1;
			cin.sync();
			auto start_time = chrono::high_resolution_clock::now();
			addNode(tree, new_node1);
			auto end_time = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
			setNodeDepth(tree);
			auto start_bal = chrono::high_resolution_clock::now();
			balanceTree(tree);
			auto end_bal = chrono::high_resolution_clock::now();
			auto total_bal = chrono::duration_cast<chrono::microseconds>(end_bal - start_bal).count();
			tree_ans << "Дерево после добавления элемента:\n";
			cout << "Время балансировки: " << total_bal << " мкс" << '\n';
			setNodeDepth(tree, false);
			cout << "Время вставки элемента: " << duration << " мкс" << endl;
			printTreeAns(tree);
			tree_ans << sep;
			break;
		}
		case 5: {
			// удаление элемента
			int data;
			stack <BST*> branch;
			cout << "Введите элемент для удаления\n>>";
			cin >> data;
			cin.sync();
			BST* new_node = findNode(tree, data, branch);
			if (new_node) {
				auto start_time = chrono::high_resolution_clock::now();
				branch.empty() ? tree = deleteNode(new_node, 0) : deleteNode(new_node, branch.top());
				auto end_time = chrono::high_resolution_clock::now();
				auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
				for (; !branch.empty();) {
					setNodeDepth(branch.top(), false);
					branch.pop();
				}
				auto start_bal = chrono::high_resolution_clock::now();
				balanceTree(tree);
				auto end_bal = chrono::high_resolution_clock::now();
				auto total_bal = chrono::duration_cast<chrono::microseconds>(end_bal - start_bal).count();
				tree_ans << "Дерево после удаления элемента:\n";
				cout << "Время балансировки: " << total_bal << " мкс" << '\n';
				setNodeDepth(tree, false);
				cout << "Время удаления элемента: " << duration << " мкс" << endl;
				printTreeAns(tree);
				tree_ans << sep;
			}
			else cout << "Node not found\n";
			break;
		}
		case 6: {
			return 0;
		}
		default: {
			key = 0;
			break;
		}
		}
		key = menu(key);
	}
	tree_f.close();
	return 0;
}